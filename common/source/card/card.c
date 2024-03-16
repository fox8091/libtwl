#include <nds.h>
#include "libtwl/card/card.h"

typedef union packedUInt_t {
    u32 value;
    u8 i[sizeof(u32)];
} PackedUInt;

void card_romCpuRead(u32* dst, u32 words)
{
    u32* target = dst + words;
    do
    {
        // Read data if available
        if (card_romIsDataReady())
        {
            u32 data = card_romGetData();
            if (dst < target)
                *dst++ = data;
        }
    } while (card_romIsBusy());
}

void NO_MEMCPY card_romCpuReadUnaligned(u8* dst, u32 words)
{
    u8* target = dst + (words << 2);
    do
    {
        // Read data if available
        if (card_romIsDataReady())
        {
            PackedUInt data;
            data.value = card_romGetData();
            if (dst < target)
            {
                for(int i = 0; i < 4; i++){
                    *dst++ = data.i[i];
                }
            }
        }
    } while (card_romIsBusy());
}

void card_romCpuWrite(const u32* src, u32 words)
{
    u32 data = 0;
    const u32* target = src + words;
    do
    {
        // Write data if ready
        if (card_romIsDataReady())
        {
            if (src < target)
                data = *src++;
            card_romSetData(data);
        }
    } while (card_romIsBusy());
}

void NO_MEMCPY card_romCpuWriteUnaligned(const u8* src, u32 words)
{
    PackedUInt data;
    const u8* target = src + (words << 2);
    do
    {
        // Write data if ready
        if (card_romIsDataReady())
        {
            if (src < target)
            {
                for(int i = 0; i < 4; i++){
                    data.i[i] = src[i];
                }
                src += 4;
            }
            card_romSetData(data);
        }
    } while (card_romIsBusy());
}
