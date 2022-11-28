#include <string.h>
#include <math.h>

#include "ose_conf.h"
#include "ose.h"
#include "ose_context.h"
#include "ose_util.h"
#include "ose_stackops.h"
#include "ose_assert.h"
#include "ose_vm.h"

#include "oscript_types.h"

#define INCP(bufp, amt) ((bufp) ? ((bufp) += (amt)) : (bufp))
#define INCL(bufp, bufl, amt) ((bufp) ? ((bufl) -= (amt)) : (bufl))

static int32_t _oscript_fmtBundle(ose_bundle bundle,
                                  int32_t offset,
                                  char *buf,
                                  int32_t buflen,
                                  int32_t indent);

static int32_t oscript_fmtBundleElem_msg(ose_bundle bundle,
                                         int32_t offset,
                                         char *buf,
                                         int32_t buflen,
                                         int32_t indent);

static int32_t oscript_fmtBundleElem(ose_bundle bundle,
                                     int32_t offset,
                                     char *buf,
                                     int32_t buflen,
                                     int32_t indent);

static int32_t oscript_fmtMessageAddr(ose_bundle bundle,
                                      int32_t offset,
                                      int32_t ao,
                                      char *buf,
                                      int32_t buflen)
{
    const char * const addr = ose_readString(bundle, ao);
    return snprintf(buf, buflen, "%s", addr);
}

#ifdef OSE_PROVIDE_TYPE_TIMETAG
// support functions for formatting OSC (NTP) timetags

// Year is a leap year if year mod 400 = 0,
// OR if (year mod 4 = 0 AND year mod 100 <> 0).
static unsigned short int isleap(unsigned short int year)
{
	short int yrtest;
	yrtest = year;

	//year modulo 400 = 0? if so, it's a leap year
	while(yrtest > 0)
    {
		yrtest -= 400;
	} 
	if(yrtest == 0)
    {
		// then year modulo 400 = 0
		return 1; //it's a leap year
	}
	yrtest = year;  

	// year modulo 4 = 0 and year modulo 100 <>0? if so,
    // it's a leap year
	while(yrtest > 0)
    {
		yrtest -= 4;
	} 
	if(yrtest == 0)
    {
		// then year modulo 4 = 0
		yrtest = year;
		while(yrtest > 0)
        {
			// so test for modulo 100
			yrtest -= 100;
		}
		if(yrtest < 0)
        {
			// then year modulo 100 <>0
			return 1; //it's a leap year
		}
        else
        {
			return 0; //not a leap year
		}
	}
    else
    {
		return 0;  //year modulo 4 <> 0, not a leap year
	}
}

static unsigned short int getmonth(unsigned short int *day,
                                   unsigned short int leap)
{
	const unsigned short int JAN = 1;
	const unsigned short int FEB = 2;
	const unsigned short int MAR = 3;
	const unsigned short int APR = 4;
	const unsigned short int MAY = 5;
	const unsigned short int JUN = 6;
	const unsigned short int JUL = 7;
	const unsigned short int AUG = 8;
	const unsigned short int SEP = 9;
	const unsigned short int OCT = 10;
	const unsigned short int NOV = 11;
	const unsigned short int DEC = 12; 
	
	unsigned short int JAN_LAST = 31;
	unsigned short int FEB_LAST = 59;
	unsigned short int MAR_LAST = 90;
	unsigned short int APR_LAST = 120;
	unsigned short int MAY_LAST = 151;
	unsigned short int JUN_LAST = 181;
	unsigned short int JUL_LAST = 212;
	unsigned short int AUG_LAST = 243;
	unsigned short int SEP_LAST = 273;
	unsigned short int OCT_LAST = 304;
	unsigned short int NOV_LAST = 334;
	unsigned short int DEC_LAST = 365;

	// correct monthly end dates for leap years (leap=1=leap
	// year, 0 otherwise)
	if(leap > 0)
    {
		if(leap <= 1)
        {
			FEB_LAST += leap;
			MAR_LAST += leap;
			APR_LAST += leap;
			MAY_LAST += leap;
			JUN_LAST += leap;
			JUL_LAST += leap;
			AUG_LAST += leap;
			SEP_LAST += leap;
			OCT_LAST += leap;
			NOV_LAST += leap;
			DEC_LAST += leap;

		}
        else if(leap > 1)
        {
			return 0;
		}
	}
    else if(leap < 0)
    {
		return 0;
	}
	// Determine month & day of month from day of year
	if(*day <= JAN_LAST)
    {
		return JAN;  // day is already correct
	}
    else if(*day <= FEB_LAST)
    {
		*day -= JAN_LAST;
		return FEB;
	}
    else if(*day <= MAR_LAST)
    {
		*day -= FEB_LAST;
		return MAR;
	}
    else if(*day <= APR_LAST)
    {
		*day -= MAR_LAST;
		return APR;
	}
    else if(*day <= MAY_LAST)
    {
		*day -= APR_LAST;
		return MAY;
	}
    else if(*day <= JUN_LAST)
    {
		*day -= MAY_LAST;
		return JUN;
	}
    else if(*day <= JUL_LAST)
    {
		*day -= JUN_LAST;
		return JUL;
	}
    else if(*day <= AUG_LAST)
    {
		*day -= JUL_LAST;
		return AUG;
	}
    else if(*day <= SEP_LAST)
    {
		*day -= AUG_LAST;
		return SEP;
	}
    else if(*day <= OCT_LAST)
    {
		*day -= SEP_LAST;
		return OCT;
	}
    else if(*day <= NOV_LAST)
    {
		*day -= OCT_LAST;
		return NOV;
	}
    else if(*day <= DEC_LAST)
    {
		*day -= NOV_LAST;
		return DEC;
	}
    else
    {
		return 0;
	}
}
#endif

static int32_t oscript_fmtMessageArg(ose_bundle bundle,
                                     int32_t offset,
                                     int32_t tto,
                                     int32_t plo,
                                     char *buf,
                                     int32_t buflen,
                                     int32_t indent)
{
    char tt = ose_readByte(bundle, tto);
    switch(tt){
    case OSETT_INT32:
        return snprintf(buf, buflen,
                        "%d",
                        ose_readInt32(bundle, plo));
    case OSETT_FLOAT:
        return snprintf(buf, buflen,
                        "%f",
                        ose_readFloat(bundle, plo));
    case OSETT_STRING:
        return snprintf(buf, buflen,
                        "\"%s\"",
                        ose_readString(bundle, plo));
    case OSETT_BLOB:
    {
        int32_t blobsize = ose_readInt32(bundle, plo);
        int32_t n =  snprintf(buf, buflen, "b");
        int32_t nn = n;
        INCP(buf, n);
        INCL(buf, buflen, n);
        const char * const p = ose_readString(bundle, plo + 4);
        for(int k = 0; k < blobsize; k++){
            n = snprintf(buf, buflen, "%02X", p[k]);
            nn += n;
            INCP(buf, n);
            INCL(buf, buflen, n);
        }
        return nn;
    }
#ifdef OSE_PROVIDE_TYPE_DOUBLE
    case OSETT_DOUBLE:
        return snprintf(buf, buflen,
                        "%f",
                        ose_readDouble(bundle, plo));
#endif
#ifdef OSE_PROVIDE_TYPE_TIMETAG
    case OSETT_TIMETAG:
    {
        const struct ose_timetag timetag =
            ose_readTimetag(bundle, plo);
        uint32_t secs = timetag.sec;
        uint32_t fsecs = timetag.fsec;
        const uint64_t SEC_PER_YEAR = 31536000;
        const uint64_t SEC_PER_DAY = 86400;
        const unsigned short int SEC_PER_HR = 3600;
        const unsigned short int SEC_PER_MIN = 60;
        char s1[20];
        unsigned short int year,
            month,
            day,
            hour,
            minute,
            leap = 0;

        for(year = 0; secs >= SEC_PER_YEAR; ++year)
        {
			// determine # years elapse since epoch
            secs -= SEC_PER_YEAR;
            if(isleap(year))
            {
                secs -= SEC_PER_DAY;
            }
        }
        year += 1900;  // 1/1/1900, 00:00 is ntp epoch
        // Is this a leap year?
        leap = isleap(year); 
        for(day = 1; secs >= SEC_PER_DAY; ++day)
        {
			// determine # days elapsed in current year
            secs -= SEC_PER_DAY;
        }
        for(hour = 0; secs >= SEC_PER_HR; ++hour)
        {
			// determine hours elapsed in current day
            secs -= SEC_PER_HR;
        }
        for(minute = 0; secs >= SEC_PER_MIN; ++minute)
        {
			// determine minutes elapsed in current hour
            secs -= SEC_PER_MIN;
        }
	
        // The value of secs at the end of the minutes loop is the
        // seconds elapsed in the current minute.  Given the year 
        // & day of year, determine month & day of month
        month = getmonth(&day, leap);
	
        //convert it all to the iso-8601 string
        {
            char yearstr[5],
                monthstr[3],
                daystr[3],
                hourstr[3],
                minutestr[3],
                secstr[3];
            snprintf(yearstr, sizeof(yearstr), "%04u", year);
            snprintf(monthstr, sizeof(monthstr), "%02u", month);
            snprintf(daystr, sizeof(daystr), "%02u", day);
            snprintf(hourstr, sizeof(hourstr), "%02u", hour);
            snprintf(minutestr, sizeof(minutestr), "%02u",
                     minute);
            snprintf(secstr, sizeof(secstr), "%02u", secs);

            //put it all together
            long i = 0;
            s1[i++] = yearstr[0];
            s1[i++] = yearstr[1];
            s1[i++] = yearstr[2];
            s1[i++] = yearstr[3];
            s1[i++] = '-';
            s1[i++] = monthstr[0];
            s1[i++] = monthstr[1];
            s1[i++] = '-';
            s1[i++] = daystr[0];
            s1[i++] = daystr[1];
            s1[i++] = 'T';
            s1[i++] = hourstr[0];
            s1[i++] = hourstr[1];
            s1[i++] = ':';
            s1[i++] = minutestr[0];
            s1[i++] = minutestr[1];
            s1[i++] = ':';
            s1[i++] = secstr[0];
            s1[i++] = secstr[1];
            s1[i++] = '\0';
        }

        double d = ((double)secs + (double)fsecs) / 4294967295.0;
        double dm1 = fmod(d, 1.0);
        int32_t l = snprintf(NULL, 0, "%05fZ", dm1);
        char s2[l + 1];
        snprintf(s2, l, "%05fZ", dm1);
        return snprintf(buf, buflen, "%s.%sZ", s1, s2+2);
    }
#endif // OSE_PROVIDE_TYPE_TIMETAG
    case OSETT_BUNDLE:
    {
        return oscript_fmtBundleElem(bundle,
                                     plo,
                                     buf,
                                     buflen,
                                     indent);
    }
    case OSCRIPT_TYPETAG_LAMBDA:
    {
        struct oscript_lambda_params p =
            oscript_lambda_getParams(bundle, plo);
        int32_t n = snprintf(buf, buflen, "<fn: (");
        int32_t nn = 0;
        INCP(buf, n);
        INCL(buf, buflen, n);
        nn += n;
        for(int32_t i = 0; i < p.nparams - 1; ++i)
        {
            n = snprintf(buf, buflen, "%s, ",
                         oscript_lambda_getParam(p, i));
            INCP(buf, n);
            INCL(buf, buflen, n);
            nn += n;
        }
        n = snprintf(buf, buflen, "%s)>",
                     oscript_lambda_getParam(p, p.nparams - 1));
        INCP(buf, n);
        INCL(buf, buflen, n);
        nn += n;
        return nn;
    }
    default:
    {
        int32_t ret = OSE_PPRINTPAYLOADITEM_HOOK(bundle,
                                                 tt,
                                                 offset,
                                                 plo);
        //char sep = SEP;
        int32_t len;
        if(ret < 0)
        {
            len = snprintf(buf, buflen, "<%c%c<>>",
                           tt, ':');
        }
        else
        {
            len = snprintf(buf, buflen, "<%c%c%s>",
                           tt, ':',
                           ose_peekString(bundle));
            ose_drop(bundle);
        }
        return len;
    }
    }
}

static int32_t oscript_fmtBundleElem_msg(ose_bundle bundle,
                                         int32_t offset,
                                         char *buf,
                                         int32_t buflen,
                                         int32_t indent)
{
    char *b = ose_getBundlePtr(bundle);
    int32_t s = ose_readInt32(bundle, offset);
    int32_t ao = offset + 4;
    int32_t tto = ao + ose_pstrlen(b + ao);
    const int32_t ntt = strlen(b + tto) - 1;
    int32_t plo = tto + ose_pstrlen(b + tto);
    tto++;
    int32_t n = 0;
    int32_t nn = 0;
    char *bufp = buf;
    int32_t bufl = buflen;
    {
        /* address */
        n = oscript_fmtMessageAddr(bundle,
                                   offset,
                                   ao,
                                   bufp,
                                   bufl);
        nn += n;
        INCP(bufp, n);
        INCL(bufp, bufl, n);
    }
    if(n)
    {
        n = snprintf(bufp, bufl, " : ");
    }
    else
    {
        n = snprintf(bufp, bufl, ": ");
    }
    nn += n;
    INCP(bufp, n);
    INCL(bufp, bufl, n);
    /* payload */
    if(ntt == 0)
    {
    }
    else if(ntt == 1)
    {
        n = oscript_fmtMessageArg(bundle,
                                  offset,
                                  tto,
                                  plo,
                                  bufp,
                                  bufl,
                                  indent);
        nn += n;
        INCP(bufp, n);
        INCL(bufp, bufl, n);
    }
    else
    {
        n = snprintf(bufp, bufl, "[ ");
        nn += n;
        INCP(bufp, n);
        INCL(bufp, bufl, n);
        while(plo < (offset + 4 + s) && ose_readByte(bundle, tto))
        {
            char tt = ose_readByte(bundle, tto);
            n = oscript_fmtMessageArg(bundle,
                                      offset,
                                      tto,
                                      plo,
                                      bufp,
                                      bufl,
                                      indent);
            nn += n;
            INCP(bufp, n);
            INCL(bufp, bufl, n);
            if(ose_readByte(bundle, tto + 1))
            {
                n = snprintf(bufp, bufl, ", ");
                nn += n;
                INCP(bufp, n);
                INCL(bufp, bufl, n);
            }
            int32_t payload_item_size =
                ose_getPayloadItemSize(bundle, tt, offset, plo);
            if(payload_item_size < 0)
            {
                /* skip the rest of the message */
                plo = offset + s + 4;
            }
            else
            {
                plo += payload_item_size;
            }
            tto++;
        }
        n = snprintf(bufp, bufl, " ]");
        nn += n;
        INCP(bufp, n);
        INCL(bufp, bufl, n);
    }
    return nn;
}

static int32_t oscript_fmtBundleElem(ose_bundle bundle,
                                     int32_t offset,
                                     char *buf,
                                     int32_t buflen,
                                     int32_t indent)
{
    int32_t nn = 0;
    char *bufp = buf;
    int32_t bufl = buflen;
    int n = 0;
    char betype = ose_getBundleElemType(bundle, offset);
    if(betype == OSETT_MESSAGE)
    {
        n = oscript_fmtBundleElem_msg(bundle,
                                      offset,
                                      bufp,
                                      bufl,
                                      indent);
        return n;
    }
    else if(betype == OSETT_BUNDLE)
    {
        if(ose_getBundleElemElemCount(bundle, offset) == 0)
        {
            n = snprintf(bufp, bufl, "{}");
            nn += n;
            INCP(bufp, n);
            INCL(bufp, bufl, n);
        }
        else
        {
            n = snprintf(bufp, bufl, "{\n");
            nn += n;
            INCP(bufp, n);
            INCL(bufp, bufl, n);
            
            n = _oscript_fmtBundle(bundle,
                                   offset + 4,
                                   bufp,
                                   bufl,
                                   indent + 1);
            nn += n;
            INCP(bufp, n);
            INCL(buf, bufl, n);

            char indentstr[indent + 1];
            for(int i = 0; i < indent; i++)
            {
                indentstr[i] = ' ';
            }
            indentstr[indent] = 0;
            n = snprintf(bufp, bufl, "\n%s}", indentstr);
            nn += n;
            INCP(bufp, n);
            INCL(buf, bufl, n);
        }
        return nn;
    }
    else
    {
        ose_assert(0 &&
                   "bundle elem is neither a message nor a bundle");
    }
    return 0;
}

static int32_t _oscript_fmtBundle(ose_bundle bundle,
                                  int32_t offset,
                                  char *buf,
                                  int32_t buflen,
                                  int32_t indent)
{
    ose_assert(ose_isBundle(bundle));
    int32_t nn = 0;
    int32_t ss = ose_readInt32(bundle, offset - 4);
    char *bufp = buf;
    int32_t bufl = buflen;
    int32_t o = OSE_BUNDLE_HEADER_LEN;
    offset += OSE_BUNDLE_HEADER_LEN;
    char indentstr[indent + 1];
    for(int i = 0; i < indent; i++)
    {
        indentstr[i] = ' ';
    }
    indentstr[indent] = 0;
    while(o < ss)
    {
        int n = 0;
        n = snprintf(bufp, bufl, "%s", indentstr);
        nn += n;
        INCP(bufp, n);
        INCL(bufp, bufl, n);

        int32_t s = ose_readInt32(bundle, offset);
        n = oscript_fmtBundleElem(bundle,
                                  offset,
                                  bufp,
                                  bufl,
                                  indent);
        nn += n;
        INCP(bufp, n);
        INCL(bufp, bufl, n);
        o += s + 4;
        offset += s + 4;
        if(o < ss)
        {
            n = snprintf(bufp, bufl, ",\n");
            INCP(bufp, n);
            INCL(bufp, bufl, n);
            nn += n;
        }
    }
    return nn;
}

static void oscript_formatOscBundle(ose_bundle bundle)
{
    int32_t o = ose_getLastBundleElemOffset(bundle);
    int32_t len = oscript_fmtBundleElem(bundle, o, NULL, 0, 0);
    len = ose_pnbytes(len);
    ose_pushBlob(bundle, len, NULL);
    char *p = ose_peekBlob(bundle) + 4;
    oscript_fmtBundleElem(bundle, o, p, len, 0);
    ose_pushInt32(bundle, OSETT_STRING);
    ose_blobToType(bundle);
}

static void oscript_formatlnOscBundle(ose_bundle bundle)
{
    int32_t o = ose_getLastBundleElemOffset(bundle);
    int32_t tlen = oscript_fmtBundleElem(bundle, o, NULL, 0, 0);
    int32_t tnrlen = tlen + 1;                   // \n
    int32_t ptnrlen = ose_pnbytes(tnrlen);
    ose_pushBlob(bundle, ptnrlen, NULL);
    char *p = ose_peekBlob(bundle) + 4;
    oscript_fmtBundleElem(bundle, o, p, ptnrlen, 0);
    p[tlen] = '\n';
    ose_pushInt32(bundle, OSETT_STRING);
    ose_blobToType(bundle);
}

static void oscript_formatOscMessage(ose_bundle bundle)
{
    int32_t o = ose_getLastBundleElemOffset(bundle);
    int32_t len = oscript_fmtBundleElem_msg(bundle, o, NULL, 0, 0);
    len = ose_pnbytes(len);
    ose_pushBlob(bundle, len, NULL);
    char *p = ose_peekBlob(bundle) + 4;
    oscript_fmtBundleElem_msg(bundle, o, p, len, 0);
    ose_pushInt32(bundle, OSETT_STRING);
    ose_blobToType(bundle);
}

static void oscript_formatlnOscMessage(ose_bundle bundle)
{
    int32_t o = ose_getLastBundleElemOffset(bundle);
    int32_t tlen = oscript_fmtBundleElem_msg(bundle, o, NULL, 0, 0);
    int32_t tnrlen = tlen + 1;  // \n
    int32_t ptnrlen = ose_pnbytes(tnrlen);
    ose_pushBlob(bundle, ptnrlen, NULL);
    char *p = ose_peekBlob(bundle) + 4;
    oscript_fmtBundleElem_msg(bundle, o, p, ptnrlen, 0);
    p[tlen] = '\n';
    ose_pushInt32(bundle, OSETT_STRING);
    ose_blobToType(bundle);
}

static void oscript_format(ose_bundle osevm)
{
    ose_bundle vm_s = OSEVM_STACK(osevm);
    char t = ose_peekType(vm_s);
    if(t == OSETT_MESSAGE)
    {
        oscript_formatOscMessage(vm_s);
    }
    else if(t == OSETT_BUNDLE)
    {
        oscript_formatOscBundle(vm_s);
    }
    else
    {
        ose_assert(0 && "found something that is neither a bundle nor a message");
    }
}

static void oscript_formatln(ose_bundle osevm)
{
    ose_bundle vm_s = OSEVM_STACK(osevm);
    if(!ose_bundleHasAtLeastNElems(vm_s, 1))
    {
        ose_pushString(vm_s, "\n");
        return;
    }
    char t = ose_peekType(vm_s);
    if(t == OSETT_MESSAGE)
    {
        oscript_formatlnOscMessage(vm_s);
    }
    else if(t == OSETT_BUNDLE)
    {
        oscript_formatlnOscBundle(vm_s);
    }
    else
    {
        ose_assert(0 && "found something that is neither a bundle nor a message");
    }
}

void oscript_print_load(ose_bundle vm_s)
{
    ose_pushMessage(vm_s, "/o/format", strlen("/o/format"),
                    1, OSETT_ALIGNEDPTR, oscript_format);
    ose_pushMessage(vm_s, "/o/formatln", strlen("/o/formatln"),
                    1, OSETT_ALIGNEDPTR, oscript_formatln);
                    
}
