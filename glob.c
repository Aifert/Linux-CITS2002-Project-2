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

int matchRegex(FLAG *flags, const char *filename, int mode)
{
	char *globPattern;
	int pattern_count;
	char **patterns;

	if (mode == 1)
	{
		pattern_count = flags->pattern_i_count;
		patterns = flags->patterns_i;
	}
	else if (mode == 0)
	{
		pattern_count = flags->pattern_o_count;
		patterns = flags->patterns_o;
	}
	else
	{
		return 0; // Invalid mode
	}

	for (int i = 0; i < pattern_count; i++)
	{
		globPattern = patterns[i];
		char *regexPattern = glob2regex(globPattern);

		if (regexPattern != NULL)
		{
			regex_t regex;
			if (regcomp(&regex, regexPattern, 0) == 0)
			{
				if (regexec(&regex, filename, 0, NULL, 0) == 0)
				{
					regfree(&regex);
					free(regexPattern);
					if (flags->flag_v)
					{
						printf("Matches '%s'\n", patterns[i]);
					}
					return 1;
				}
				regfree(&regex);
			}
			free(regexPattern);
		}
	}

	return 0;
}
