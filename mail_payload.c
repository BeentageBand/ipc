#define COBJECT_IMPLEMENTATION
#define Dbg_FID DBG_FID_DEF(IPC_FID, 3)

#include "dbg_log.h"
#include "cobject.h"

#define MAIL_PAYLOAD_BIG_BLOCK_SIZE (1024)
#define MAIL_PAYLOAD_MID_BLOCK_SIZE (256)
#define MAIL_PAYLOAD_LIL_BLOCK_SIZE (32)
#define MAIL_PAYLOAD_SIZE           (256)

typedef struct Class Payload_Class_T;

union Payload
{
        struct
        {
                struct Object Object;
                uint8_t * ptr;
                size_t block_size;
                size_t i;
                size_t end;
        };
        Payload_Class_T * vtbl;
};


static void payload_delete(struct Object * const obj);
static size_t payload_alloc(union Payload * const this, void ** const payload);
static size_t payload_free(union Payload * const this, void ** const payload);

static Payload_Class_T Payload_Class =
{
        payload_delete,
        NULL
};
static uint8_t Big_Pool_Buff[MAIL_PAYLOAD_SIZE * MAIL_PAYLOAD_BIG_BLOCK_SIZE];
static uint8_t Mid_Pool_Buff[MAIL_PAYLOAD_SIZE * MAIL_PAYLOAD_MID_BLOCK_SIZE];
static uint8_t Lil_Pool_Buff[MAIL_PAYLOAD_SIZE * MAIL_PAYLOAD_LIL_BLOCK_SIZE];

static union Payload Big_Pool =
{{
        {&Payload_Class},
        {payload_delete, NULL},
        Big_Pool_Buff,
        MAIL_PAYLOAD_BIG_BLOCK_SIZE,
        0,
        Num_Elems(Big_Pool_Buff)
}};

static union Payload Mid_Pool =
{{
        {&Payload_Class},
        Mid_Pool_Buff,
        MAIL_PAYLOAD_MID_BLOCK_SIZE,
        0,
        Num_Elems(Mid_Pool_Buff)
}};
static union Payload Lil_Pool =
{{
        {&Payload_Class},
        Lil_Pool_Buff,
        MAIL_PAYLOAD_LIL_BLOCK_SIZE,
        0,
        Num_Elems(Lil_Pool_Buff)
}};


void payload_delete(struct Object * const obj)
{

}

size_t payload_alloc(union Payload * const this, void ** const payload)
{
    if(this->i < this->end)
    {
        Dbg_Fault("Overflow on");
        return 0;
    }

    this->vtbl = &Payload_Class;
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
    return pay_size;
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
    return rc;
}
