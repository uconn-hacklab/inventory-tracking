#include "tm_reader.h"
#include "tmr_status.h"
#include "tmr_strerror.h"
#include <string.h>
#include <stdio.h>

#define BAUD_RATE 115200

TMR_Reader reader, *reader_p;
TMR_String model;
char string[100];

static void checkerr(TMR_Reader *reader_p, TMR_Status ret, int exitval, const char *msg)
{
  if (TMR_SUCCESS != ret)
  {

    // output hex printf 
    printf("ERROR %s: 0x%04x", msg, ret); 
    printf(":\n");
    printf("%s", TMR_strerror(ret));
    // while (1)
    // {
    //   blink(ERROR_BLINK_COUNT, ERROR_BLINK_INTERVAL);
    // }
  }
}


static void initializeReader() {
  TMR_Status ret;
  TMR_ReadPlan plan;
  uint8_t antennaList[] = {1};
  uint8_t antennaCount = 1;
  TMR_TagProtocol protocol;

  reader_p = &reader;
  
  // tmr indicates that the API will decide to connect with
  // EAPI or LLRP protocol
  // the M6e module is EAPI, so connects to a SerialReader device type
  // No authority (local system)
  ret = TMR_create(reader_p, "tmr:///Serial1");
  checkerr(reader_p, ret, 1, "creating reader");

  ret = TMR_connect(reader_p);
  checkerr(reader_p, ret, 1, "connecting reader");

  model.value = string;
  model.max = sizeof(string);
  TMR_paramGet(reader_p, TMR_PARAM_VERSION_MODEL, &model);
  checkerr(reader_p, ret, 1, "Getting version model");

  /* Set region to North America */
  if (0 != strcmp("M3e", model.value)) {
    TMR_Region region = TMR_REGION_NA;
    ret = TMR_paramSet(reader_p, TMR_PARAM_REGION_ID, &region);
    checkerr(reader_p, ret, 1, "setting region");
  }

  /* Set protocol */
  if (0 != strcmp("M3e", model.value)) {
    protocol = TMR_TAG_PROTOCOL_GEN2;
  } else {
    protocol = TMR_TAG_PROTOCOL_ISO14443A;
  }

#if SET_M6E_COMPATIBLE_PARAMS
  {
/* To make this code compatible with M6e family modules,
 * set below configurations.
 */
#if ENABLE_CONTINUOUS_READ
    {
      {
        /* 1. Disable read filter: To report repeated tag entries of same tag,
         * users must disable read filter for continuous read. This filter is
         * enabled by default in the M6e family modules. Note that this is a one
         * time configuration while connecting to the module after power ON. We
         * do not have to set it in every read cycle.
         */
        bool readFilter = false;
        ret = TMR_paramSet(reader_p, TMR_PARAM_TAGREADDATA_ENABLEREADFILTER,
                           &readFilter);
        checkerr(reader_p, ret, 1, "setting read filter");
      }

      {
        /* 2. Metadata flag: TMR_TRD_METADATA_FLAG_ALL includes all flags
         * (Supported by UHF and HF/LF readers). Disable unsupported flags for
         * M6e family as shown below. Note that Metadata flag must be set once
         * after connecting to the module using TMR_connect().
         */
        TMR_TRD_MetadataFlag metadata =
            (uint16_t)(TMR_TRD_METADATA_FLAG_ALL &
                       (~TMR_TRD_METADATA_FLAG_TAGTYPE));
        ret = TMR_paramSet(reader_p, TMR_PARAM_METADATAFLAG, &metadata);
        checkerr(reader_p, ret, 1, "Setting Metadata Flags");
      }
    }
#else
    {
      /* 1. Enable read filter: This step is optional in case of Timed Reads
       * because read filter is enabled by default in the M6e family modules.
       *                     But if we observe multiple entries of the same tag
       * in the tag reports, then the read filter might have been disabled
       * prevoiusly. So, we must enable the read filter.
       */
      bool readFilter = true;
      ret =
          TMR_paramSet(reader_p, TMR_PARAM_TAGREADDATA_ENABLEREADFILTER, &readFilter);
      checkerr(reader_p, ret, 1, "setting read filter");
    }
#endif
  }
#endif /* SET_M6E_COMPATIBLE_PARAMS */

  /* Initializing the Read Plan */
  TMR_reader_p_init_simple(&plan, antennaCount, antennaList, protocol, 100);

  /* Commit the Read Plan */
  ret = TMR_paramSet(reader_p, TMR_PARAM_READ_PLAN, &plan);
  checkerr(reader_p, ret, 1, "setting read plan");
}
