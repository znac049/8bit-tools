#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "bda.h"

static CFGSection *cfg = NULL;
static CFGSection *cur_sect = NULL;

CFGItem *parse_line(char *str) {
  int argc = 0;
  char *cp, *arg;
  char *argv[MAXARGS];
  CFGItem *item;
  int i;
  char *argstr = NULL;

  printf("PARSE: '%s'\n", str);
  if (*str == EOS) {
    return NULL;
  }

  str = strdup(str);
  argstr = arg = cp = skip_blanks(str);

  while (*argstr && !IS_WHITE(*argstr)) {
    argstr++;
  }
  argstr = skip_blanks(argstr);
  argstr = strdup(argstr);

  while (*arg != EOS) {
    while (*cp && !(IS_WHITE(*cp))) {
      cp++;
    }
    *cp++ = EOS;

    argv[argc] = arg;
    argc++;
    arg = cp = skip_blanks(cp);
  }

  item = malloc(sizeof(CFGItem));
  item->next = NULL;
  item->name = argv[0];
  item->argstr = argstr;
  item->argv = malloc(sizeof(char *) * (argc-1));

  for (i=1; i<argc; i++) {
    item->argv[i-1] = argv[i];
  }
  item->argc = argc-1;

  return item;
}

CFGSection *find_cfg_section(const char *section_name) {
  CFGSection *sect = cfg;

  while (sect != NULL) {
    if (strcasecmp(section_name, sect->name) == 0) {
      return sect;
    }

    sect = sect->next;
  }

  return NULL;
}

CFGItem *find_cfg_item(const char *item_name) {
  CFGItem *item = cur_sect->items;

  while (item != NULL) {
    if (strcasecmp(item_name, item->name) == 0) {
      return item;
    }

    item = item->next;
  }

  return NULL;
}

void create_cfg_section(const char *section_name) {
  CFGSection *sect = find_cfg_section(section_name);

  if (sect == NULL) {
    sect = malloc(sizeof(CFGSection));

    sect->name = strdup(section_name);
    sect->items = NULL;
    sect->next = cfg;
    cur_sect = cfg = sect;
  }
}

CFGSection *set_cfg_section(const char *section_name) {
  CFGSection *sect = find_cfg_section(section_name);

  if (sect != NULL) {
    cur_sect = sect;
  }

  return sect;
}

CFGSection *read_cfg_file(const char *config_path) {
  char line[MAXLINE];
  FILE *fd;
  char *str;
  CFGItem *item;
  CFGSection *default_sect = find_cfg_section("*");

  if (default_sect == NULL) {
    create_cfg_section("*");
  }
  set_cfg_section("*");

  if (!is_file(config_path)) {
    fprintf(stderr, "No machine file found: '%s'\n", config_path);
    return NULL;
  }

  fd = fopen(config_path, "r");
  if (fd == NULL) {
    fprintf(stderr, "Can't open config file: '%s'\n", config_path);
    return NULL;
  }

  /* Read the config file */
  str = fgets(line, MAXSTR, fd);
  while (str != NULL) {
    int len;

    strip_nl(str);
    rtrim(str);

    len = strlen(str);

    if ((str[0] == '[') && (str[len-1] == ']')) {
      str[len-1] = EOS;
      str++;

      create_cfg_section(str);
    }
    else {
      if (len) {
	item = parse_line(str);

	item->next = cur_sect->items;
	cur_sect->items = item;
      }
    }

    str = fgets(line, MAXLINE, fd);
  }

  fclose(fd);

  return cfg;
}

void dump_cfg() {
  CFGSection *sect = cfg;
  CFGItem *item;
  int i;

  printf("\nParsed config:\n");
  while (sect != NULL) {
    printf("[%s]\n", sect->name);

    item = sect->items;
    while (item != NULL) {
      printf("  %s: %d\n", item->name, item->argc);
      for (i=0; i<item->argc; i++) {
	printf("      %d: '%s'\n", i, item->argv[i]);
      }
      printf("      argstr: '%s'\n", item->argstr);
      item = item->next;
    }
    printf("\n");

    sect = sect->next;
  }
}
