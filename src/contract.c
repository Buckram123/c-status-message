typedef unsigned long long u64;
const char STATE_KEY[5] = "STATE";
const char ACCOUNT_ID[] = "\"account_id\":";
const char MESSAGE[] = "\"message\":";
const u64 LEN = 32;
const u64 LEN_U64 = 4;
const unsigned LEN_U64_USIZE = (unsigned)LEN_U64;
// #############
// # Registers #
// #############
void read_register(u64 register_id, u64 ptr);
u64 register_len(u64 register_id);

void write_register(u64 register_id, u64 data_len, u64 data_ptr);
// ###############
// # Context API #
// ###############
void predecessor_account_id(u64 register_id);
void input(u64 register_id);

void sha256(u64 value_len, u64 value_ptr, u64 register_id);

void value_return(u64 value_len, u64 value_ptr);

u64 storage_write(
    u64 key_len,
    u64 key_ptr,
    u64 value_len,
    u64 value_ptr,
    u64 register_id);
u64 storage_read(u64 key_len, u64 key_ptr, u64 register_id);
u64 storage_remove(u64 key_len, u64 key_ptr, u64 register_id);
u64 storage_has_key(u64 key_len, u64 key_ptr);
void panic();

char *ft_strstr(const char *str, const char *to_find)
{
    unsigned int pos;
    unsigned int i;

    if (!*to_find)
        return ((char *)str);
    pos = 0;
    while (str[pos] != '\0')
    {
        if (str[pos] == to_find[0])
        {
            i = 1;
            while (to_find[i] != '\0' && str[pos + i] == to_find[i])
                ++i;
            if (to_find[i] == '\0')
                return ((char *)&str[pos]);
        }
        ++pos;
    }
    return (0);
}

char *ft_strchr(const char *s, int c)
{
    while (*s)
    {
        if (*s == c)
            return ((char *)s);
        ++s;
    }
    if (c == '\0')
        return ((char *)s);
    return (0);
}

void init() // acts like a default
{
    if (storage_has_key(sizeof(STATE_KEY), (u64)&STATE_KEY) == 1)
    {
        panic();
    }
    storage_write(sizeof(STATE_KEY), (u64)&STATE_KEY, 0, 0, 0);
}

void set_status()
{
    input(0);

    u64 len = register_len(0);
    char buf[len + 1];
    read_register(0, (u64)&buf);
    buf[len + 1] = '\0';
    char *message;
    if ((message = ft_strstr(buf, MESSAGE)) == 0)
    {
        panic();
    }
    message = ft_strchr(message + sizeof(MESSAGE) - 1, '"') + 1;
    u64 message_len = ft_strchr(message, '"') - message;
    predecessor_account_id(1);
    sha256(__UINT64_MAX__, 1, 2);
    storage_write(__UINT64_MAX__, 2, message_len, (u64) message, 3);
    value_return(__UINT64_MAX__, 3);
}

void get_status()
{
    input(0);
    u64 len = register_len(0);
    char buf[len + 1];
    read_register(0, (u64)&buf);
    buf[len + 1] = '\0';
    char *message;
    if ((message = ft_strstr(buf, ACCOUNT_ID)) == 0)
    {
        panic();
    }
    message = ft_strchr(message + sizeof(ACCOUNT_ID) - 1, '"') + 1;
    u64 message_len = ft_strchr(message, '"') - message;
    sha256(message_len, (u64)message, 1);
    u64 val = storage_read(__UINT64_MAX__, 1, 2);
    if (val)
    {
        value_return(__UINT64_MAX__, 2);
    }
    else
    {
        value_return(__UINT64_MAX__, 1);
    }
}