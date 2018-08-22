zx_set_timetone("GMT-8");

int zx_set_timetone(char *timetone)
{
	if (timetone)
	{
		int index = 0;
		dzlog_info("input timetone %s", timetone);
		if (strlen(timetone) > SMALL_STR_LEN)
			return -1;

		char buf[SMALL_STR_LEN];
		char tmp[SMALL_STR_LEN];
		memset(buf, 0, sizeof(buf));
		memset(tmp, 0, sizeof(buf));
		strcpy(tmp, timetone);

		if (strncmp(tmp,"GMT", 3) == 0)
		{
			char *p = strchr(tmp, ':'); // GMT+8:00
			if (p)
			{
				*p = 0;	// 截断
				if (tmp[3] == '+')	// TZ的值同APP相反
				{
					tmp[3] = '-';
				}
				else if (tmp[3] == '-')
				{
					tmp[3] = '+';
				}
			}
			sprintf(buf, "TZ=%s", tmp);
		}
		else
		{
			sprintf(buf, "TZ=%s", tmp);
		}

		//dzlog_info("env timetone %s", buf);
		if (putenv(buf) == -1)
		{
	  		dzlog_error("putenv failed.");
			return -1;
		}
    	tzset();
		//dzlog_info("daylight = %d", daylight );
   		//dzlog_info("timezone = %ld", timezone );
   		//dzlog_info("tzname[0] = %s", tzname[0] );
	#if 0
		index = getNvramIndex("2860");	
		if( index  == -1 )
		{
			dzlog_error("getNvramIndex failed.");
			return -2;
		}
		char *ptz = nvram_bufget(index, "TZ");
		if (strcmp(ptz,tmp) != 0)
		{
			dzlog_info("nvram old TZ(%s)->new TZ(%s)",ptz, tmp);
			nvram_set(index, "TZ", tmp);

			FILE *fp = fopen("/etc/TZ", "wb");
			if (fp != NULL)
			{
				fwrite(tmp, 1, strlen(tmp), fp);
				tmp[0] = '\n';	// 要加换行符才可能识别
				fwrite(tmp, 1, 1, fp);
				fclose(fp);
			}
		}
#endif
		return 0;
	}
	return ERROR_NULL_POINT;
}