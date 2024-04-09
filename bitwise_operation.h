#define setbit(reg,bit) reg|=(1<<bit)
#define clearbit(reg,bit) reg&=(~(1<<bit))
#define get_bit(reg,bit) reg &=(1<<bit)
#define toggle(reg,bit) reg^= (1<<bit)
