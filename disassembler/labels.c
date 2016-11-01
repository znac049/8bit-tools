#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "bda.h"

extern int verbose;

Label *LabelList = NULL;

void dump_labels() {
  Label *lab = LabelList;

  if (LabelList == NULL) {
    if (verbose) {
      printf("; No labels\n\n");
    }

    return;
  }

  printf("; Labels:\n;\n");

  while (lab != NULL) {
    if ((lab->address < 0x6000) || (lab->address > 0x7fff)) {
      if (lab->name == NULL) {
	char l[MAXSTR];
	snprintf(l, MAXSTR-1, "L%04x", lab->address);
	printf("; %-21s = $%04x\n", l, lab->address);
      }
      else {
	printf("; %-21s = $%04x\n", lab->name, lab->address);
      }
    }

    lab = lab->next;
  }

  printf("\n");
}

int is_label(unsigned short addr) {
  Label *lab = LabelList;

  while (lab != NULL) {
    if (lab->address == addr) {
      return 1;
    }

    lab = lab->next;
  }

  return 0;
}

void lookup_label(int addr, char *lab_str, size_t lab_len) {
  Label *lab = LabelList;

  while (lab != NULL) {
    if (lab->address == addr) {
      if (lab->name != NULL) {
	snprintf(lab_str, lab_len, "%s", lab->name);
      }
      else {
	snprintf(lab_str, lab_len, "L%04x", lab->address);
      }

      return;
    }

    lab = lab->next;
  }

  snprintf(lab_str, lab_len, "$%04X", addr);  
}

void create_label(char *label, unsigned short addr) {
  Label *lab = LabelList;

  while (lab != NULL) {
    if (lab->address == addr) {
      if (label != NULL) {
	if (lab->name != NULL) {
	  free(lab->name);
	  lab->name = strdup(label);
	}
      }

      return;
    }

    lab = lab->next;
  }

  lab = malloc(sizeof(Label));
  lab->address = addr;
  lab->name = label;
  if (label != NULL) {
    lab->name = strdup(label);
  }

  lab->next = LabelList;
  LabelList = lab;
}

void process_line(char *line) {
  char *eq = strchr(line, '=');
  char *lab = line;
  char *val;

  if (eq != NULL) {
    unsigned short addr;

    *eq++ = EOS;
    val = eq;
    trim(lab);
    trim(val);

    addr = parse_address(val);
    create_label(lab, addr);
  }
  else {
    if (strcmp(line, "") != 0) {
      printf("; Crappy line in label file: '%s'\n", line);
    }
  }
}

int read_label_file(const char *file_name) {
  int num_labels = 0;
  FILE *fd = fopen(file_name, "r");
  char line[MAXSTR];

  if (!fd) {
    return -1;
  }

  while (fgets(line, MAXSTR-1, fd) != NULL) {
    char *semi = strchr(line, ';');

    trunc_str(line, '\n');
    trunc_str(line, ';');

    process_line(line);
  }

  fclose(fd);

  return num_labels;
}
