/* address_stack.c */
extern unsigned short AddressStack[(64*1024)];
extern int InsertInd;
extern int get_address_stack_size(void);
extern unsigned short get_address_stack_at(int addr);
extern int is_entry_point(unsigned short addr);
extern void stack_address(unsigned short addr);
/* bda.c */
extern char hex_output;
extern int verbose;
extern int cpu;
extern char src_file[512];
extern char label_file[512];
extern unsigned char byteStack[16];
extern int byteSP;
extern int byteAddr;
extern unsigned short wordStack[8];
extern int wordSP;
extern int wordAddr;
extern int (*disassemble)(int, OutputItem *);
extern void usage_helper(char *str);
extern void usage(void);
extern void parse_args(int argc, char *argv[]);
extern void clear_output_item(OutputItem *oi);
extern void select_cpu(char *str);
extern void set_starting_address(int vector, char *lab);
extern void set_starting_point(void);
extern void pass1(void);
extern void push_byte(unsigned char b, int addr);
extern void push_word(unsigned short w, int addr);
extern void emit_bytes(int *addr, unsigned char *stack, int *sp);
extern void emit_words(int *addr, unsigned short *stack, int *sp);
extern void generate_listing(void);
extern int main(int argc, char *argv[]);
/* config.c */
extern CFGItem *parse_line(char *str);
extern CFGSection *find_cfg_section(const char *section_name);
extern CFGItem *find_cfg_item(const char *item_name);
extern void create_cfg_section(const char *section_name);
extern CFGSection *set_cfg_section(const char *section_name);
extern CFGSection *read_cfg_file(const char *config_path);
extern void dump_cfg(void);
/* engine-6502.c */
extern int disassemble_6502(int addr, OutputItem *res);
/* engine-6809.c */
extern OPcode opcode_table_10[];
extern OPcode opcode_table_11[];
extern int disassemble_6809(int addr, OutputItem *res);
/* labels.c */
extern Label *LabelList;
extern void dump_labels(void);
extern int is_label(unsigned short addr);
extern void lookup_label(int addr, char *lab_str, size_t lab_len);
extern void create_label(char *label, unsigned short addr);
extern void process_line(char *line);
extern int read_label_file(const char *file_name);
/* machines.c */
extern int process_machine_file(void);
extern int read_machine_file(const char *machine_name);
extern void create_catbox_labels(void);
extern void create_ad_labels(void);
/* memory.c */
extern unsigned char Memory[(64*1024)];
extern unsigned char MemoryType[(64*1024)];
extern void init_memory(void);
extern void dump_memory_info(void);
extern int read_file(const char *file_name, int addr);
extern unsigned char get_byte(int addr);
extern void set_byte(int addr, unsigned char b);
extern unsigned short get_word(int addr);
extern int memory_is_word(int addr, int nWords);
extern int memory_is_code(int addr, int nBytes);
extern unsigned char get_memory_type(int addr);
/* utils.c */
extern int is_hex_char(char c);
extern int hex_val(char c);
extern unsigned short hex2int(char *str, unsigned short dfl);
extern unsigned short parse_address(char *str);
extern unsigned char is_file(const char *path);
extern char *trunc_str(char *s, char c);
extern char *strip_nl(char *str);
extern void ltrim(char *s);
extern void rtrim(char *s);
extern void trim(char *s);
extern char *str_pop(char **str);
extern char *skip_blanks(char *str);
