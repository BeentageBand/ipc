#define COBJECT_IMPLEMENTATION

#include "ipc_mailist_ids.h"
#include "cobject.h"

typedef struct Class Payload_Class_T;

union Payload
{
	Payload_Class_T * vtbl;
	struct
	{
		struct Object Object;
		void * ptr;
		size_t block_size;
		size_t i;
		size_t end;
	};
};


static void payload_delete(struct Object * const obj);
static size_t payload_alloc(union Payload * const this, void ** const payload);
static size_t payload_free(union Payload * const this, void ** const payload);

static Payload_Class_T Payload_Class;
static uint8_t Big_Pool_Buff[MAIL_PAYLOAD_SIZE * MAIL_PAYLOAD_BIG_BLOCK_SIZE];
static uint8_t Mid_Pool_Buff[MAIL_PAYLOAD_SIZE * MAIL_PAYLOAD_MID_BLOCK_SIZE];
static uint8_t Lil_Pool_Buff[MAIL_PAYLOAD_SIZE * MAIL_PAYLOAD_LIL_BLOCK_SIZE];

static struct Payload Big_Pool =
{
	{payload_delete, NULL},
	Big_Pool_Buff,
	MAIL_PAYLOAD_BIG_BLOCK_SIZE,
	0,
	Num_Elems(Big_Pool_Buff)
};

static struct Payload Mid_Pool =
{
	{payload_delete, NULL},
	Mid_Pool_Buff,
	MAIL_PAYLOAD_MID_BLOCK_SIZE,
	0,
	Num_Elems(Mid_Pool_Buff)
};
static struct Payload Lil_Pool =
{
	{payload_delete, NULL},
	Lil_Pool_Buff,
	MAIL_PAYLOAD_LIL_BLOCK_SIZE,
	0,
	Num_Elems(Lil_Pool_Buff)
};


size_t payload_alloc(union Payload * const this, void ** const payload)
{
	if(this->i < this->end) 
	{
		Dbg_Fault("Overflow on");
		return 0;
	}

	size_t i = this->end - this->block_size;
	while(i)
	{
		if(0 == this->ptr[i] &&
		   0 == memcmp(this->ptr + i, this->ptr + i + 1, this->block_size - 1U) )
		{
			*payload = (this->ptr + i);
			++this->i;
			break;
		}

		i -= this->block_size;
	}

	return i;
}

size_t payload_free(union Payload * const this, void ** const payload)
{
	if(0 == this->i) return 0;

	size_t i = this->end - this->block_size;

	while(i)
	{
		if(*payload == (this->ptr + i))
		{
			memset(*payload, 0, this->block_size);
			*payload = 0;
			--this->i;
			break;
		}
	}

	return i;
}

size_t Mail_Payload_Alloc(void ** const payload, size_t const pay_size)
{
	if(pay_size > 0)
	{
		if(pay_size <= MAIL_PAYLOAD_LIL_BLOCK_SIZE)
		{
			payload_alloc(&Lil_Pool, payload);
		}
		else if(pay_size <= MAIL_PAYLOAD_MID_BLOCK_SIZE)
		{
			payload_alloc(&Mid_Pool, payload);
		}
		else if(pay_size <= MAIL_PAYLOAD_BIG_BLOCK_SIZE)
		{
			payload_alloc(&Big_Pool, payload);
		}
		else
		{
			Dbg_Fault("Pay size too big for current block %l", pay_size);
		}
	}
	else 
	{
		/* Do Nothing */
	}
}

size_t Mail_Payload_Free(void ** const payload)
{
	size_t rc = payload_free(&Lil_Pool, payload);

	if(rc)
	{
		rc = payload_free(&Mid_Pool, payload);

		if(rc)
		{
			rc = payload_free(&Big_Pool, payload);

			if(rc)
			{
				Dbg_Fault("No payload was freed");
			}
				
		}
	}
}
