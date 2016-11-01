/* address_stack.c */
int get_address_stack_size(void);
unsigned short get_address_stack_at(int addr);
int is_entry_point(unsigned short addr);
void stack_address(unsigned short addr);
/* bda.c */
void usage_helper(char *str);
void usage(void);
void parse_args(int argc, char *argv[]);
void clear_output_item(OutputItem *oi);
void select_cpu(char *str);
void set_starting_address(int vector, char *lab);
void set_starting_point(void);
void pass1(void);
void push_byte(unsigned char b, int addr);
void push_word(unsigned short w, int addr);
void emit_bytes(int *addr, unsigned char *stack, int *sp);
void emit_words(int *addr, unsigned short *stack, int *sp);
void generate_listing(void);
int main(int argc, char *argv[]);
/* engine-6502.c */
int disassemble_6502(int addr, OutputItem *res);
/* engine-6809.c */
int disassemble_6809(int addr, OutputItem *res);
/* labels.c */
void dump_labels(void);
int is_label(unsigned short addr);
void lookup_label(int addr, char *lab_str, size_t lab_len);
void create_label(char *label, unsigned short addr);
void process_line(char *line);
int read_label_file(const char *file_name);
/* machines.c */
void parse_line(char *str);
void read_machine_file(const char *machine_name);
void create_catbox_labels(void);
void create_ad_labels(void);
/* memory.c */
void init_memory(void);
void dump_memory_info(void);
int read_file(const char *file_name, int addr);
unsigned char get_byte(int addr);
void set_byte(int addr, unsigned char b);
unsigned short get_word(int addr);
int memory_is_word(int addr, int nWords);
int memory_is_code(int addr, int nBytes);
unsigned char get_memory_type(int addr);
/* utils.c */
int is_hex_char(char c);
int hex_val(char c);
unsigned short hex2int(char *str, unsigned short dfl);
unsigned short parse_address(char *str);
unsigned char is_file(char *path);
char *trunc_str(char *s, char c);
char *strip_nl(char *str);
void ltrim(char *s);
void rtrim(char *s);
void trim(char *s);
char *str_pop(char **str);
char *skip_blanks(char *str);
