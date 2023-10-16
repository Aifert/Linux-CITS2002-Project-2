#include "glob.h"

char *glob2regex(char *glob)
{
	char *re = NULL;

	if (glob != NULL)
	{
		re = calloc(strlen(glob) * 2 + 4, sizeof(char));
		if (re == NULL)
		{
			return NULL;
		}

		char *r = re;

		*r++ = '^';
		while (*glob != '\0')
			switch (*glob)
			{
			case '.':
			case '\\':
			case '$':
				*r++ = '\\';
				*r++ = *glob++;
				break;
			case '*':
				*r++ = '.';
				*r++ = *glob++;
				break;
			case '?':
				*r++ = '.';
				glob++;
				break;
			case '/':
				free(re);
				re = NULL;
				break;
			default:
				*r++ = *glob++;
				break;
			}
		if (re)
		{
			*r++ = '$';
			*r = '\0';
		}
	}
	return re;
}

int matchRegex(char *globPattern, const char *filename)
{
	char *regexPattern = glob2regex(globPattern);

	if (regexPattern != NULL)
	{
		regex_t regex;
		if (regcomp(&regex, regexPattern, 0) == 0)
		{
			if (regexec(&regex, filename, 0, NULL, 0) == 0)
			{
				return 1;
			}
			regfree(&regex);
		}
	}
	else
	{
		free(regexPattern);
	}
	return 0;
}