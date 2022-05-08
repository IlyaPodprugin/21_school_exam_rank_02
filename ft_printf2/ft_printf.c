#include "./ft_printf.h"

typedef struct s_list
{
	int width;
	int point;
	int precis;
} t_list;

t_list init_struct(t_list flags)
{
	flags.width = 0;
	flags.point = 0;
	flags.precis = 0;
	return (flags);
}

int ft_isdigit(char c)
{
	return (c >= '0' && c <= '9');
}

int is_spec(char c)
{
	return (c == 's' || c == 'd' || c == 'x');
}

int ft_strlen (char *str)
{
	int i = 0;

	while (str[i])
		i++;
	return (i);
}

int print_width(int size, char c)
{
	int count = 0;

	while (count < size)
	{
		write(1, &c, 1);
		count++;
	}
	return (count);
}

int ft_putstr(char *str, int size)
{
	int count = 0;

	while (count < size)
	{
		write(1, &str[count], 1);
		count++;
	}
	return (count);
}

int check_size(int number)
{
	unsigned int num;
	int count = 0;

	if (number == 0)
		return (1);
	if (number < 0)
	{
		count++;
		num = number * (-1);
	}
	else
		num = number;
	while (num > 0)
	{
		count++;
		num /= 10;
	}
	return (count);
}

char *get_num(int number, int size)
{
	unsigned num;
	char *str;

	str = (char *)malloc(sizeof(char) * (size + 1));
	str[size] = '\0';
	if (number == 0)
	{
		str[0] = '0';
		return (str);
	}
	if (number < 0)
		num = number * (-1);
	else
		num = number;
	while (size > 0)
	{
		str[size - 1] = (num % 10) + '0';
		num /= 10;
		size--;
	}
	if (number < 0)
		str[size] = '-';
	return (str);
}

int print_int(t_list flags, va_list value)
{
	int count = 0;
	int spaces = 0;
	int number;
	char *str;
	int size;

	number = va_arg(value, int);
	size = check_size(number);
	str = get_num(number, size);
	if (flags.precis == 0 && flags.point == 1 && number == 0)
	{
		free(str);
		str = "";
	}
	size = ft_strlen(str);
	if (number < 0)
		size--;
	if (flags.precis > size)
	{       
		if (number < 0)
			spaces = flags.precis + 1;
		else
			spaces = flags.precis;
	}       
	else
	{
		if (number < 0)
			spaces = size + 1;
		else
			spaces = size;
	}
	count += print_width(flags.width - spaces, ' ');
	if (number < 0)
		count += print_width(1, '-');
	count += print_width(flags.precis - size, '0');
	count += ft_putstr(((number < 0) ? &str[1] : &str[0]), size);
	if (!(flags.precis == 0 && flags.point == 1 && number == 0))
		free(str);
	return (count);
}

int print_str(t_list flags, va_list value)
{
	char *str;
	int size;
	int count = 0;

	str = va_arg(value, char *);
	if (!str)
	{
		if (flags.precis < 6 && flags.precis >= 0 && flags.point == 1)
			str = "";
		else
			str = "(null)";
	}
	if (flags.precis == 0 && flags.point == 1)
		str = "";
	size = ft_strlen(str);
	count += print_width(flags.width - ((flags.precis > 0 && flags.precis < size) ? flags.precis : size), ' ');
	count += ft_putstr(str, ((flags.precis > 0 && flags.precis < size) ? flags.precis : size));
	return (count);
}

int	count_size(unsigned int digit)
{
	int	counter = 0;

	if (digit == 0)
		return (1);
	while (digit != 0)
	{
		digit /= 16;
		counter++;
	}
	return (counter);
}

void	fill_result(char *res, int size, unsigned int digit)
{
	if (digit == 0)
		res[0] = '0';
	while (digit != 0)
	{
		if (digit % 16 >= 10)
			res[size - 1] = digit % 16 + 87;
		else
			res[size - 1] = digit % 16 + 48;
		digit /= 16;
		size--;
	}
}

int	ft_print_hex(t_list flags, va_list value)
{
	int	len = 0;
	char	*res;
	unsigned int	digit = va_arg(value, unsigned int);
	int	size = count_size(digit);

	if (flags.precis == 0 && flags.point == 1 && digit == 0)
	{
		size = 0;
		res = "";
	}
	len += print_width(flags.width - ((flags.precis > size) ? flags.precis : size), ' ');
	len += print_width(flags.precis - size, '0');
	if (!(flags.precis == 0 && flags.point == 1 && digit == 0))
	{
		res = (char *)malloc(sizeof(char) * (size + 1));
		if (!res)
			return (0);
		res[size] = '\0';
		fill_result(res, size, digit);
		len += ft_putstr(res, size);
		free(res);
	}

	return (len);
}

t_list pars_flags(t_list flags, const char *str)
{
	int i = 0;

	while (str[i] && (is_spec(str[i]) == 0))
	{
		if (ft_isdigit(str[i]) && flags.point == 0)
			flags.width = (flags.width * 10) + (str[i] - '0');
		else if (str[i] == '.' && flags.point == 0)
			flags.point = 1;
		else if (ft_isdigit(str[i]) && flags.point == 1)
			flags.precis = (flags.precis * 10) + (str[i] - '0');
		i++;
	}
	return (flags);
}

int 	sort_spec(char c, t_list flags, va_list value)
{
	int count = 0;

	if (c == 'd')
		count += print_int(flags, value);
	if (c == 's')
		count += print_str(flags, value);
	if (c == 'x')
		count += ft_print_hex(flags, value);
	return (count);
}

int ft_printf(const char *str, ...)
{
	int count = 0;
	va_list value;
	int i = 0;
	t_list flags;

	va_start(value, str);
	while (str[i])
	{
		if (str[i] == '%')
		{
			i++;
			flags = init_struct(flags);
			flags = pars_flags(flags, &str[i]);
			while (is_spec(str[i]) == 0)
				i++;
			count += sort_spec(str[i], flags, value);
		}
		else
		{
			write(1, &str[i], 1);
			count++;
		}
		i++;
	}
	va_end(value);
	return (count);
}

// int	main(void)
// {
// 	ft_printf(" %x ", 0);
// 	ft_printf(" %x ", -1);
// 	ft_printf(" %x ", 1);
// 	ft_printf(" %x ", 9);
// 	ft_printf(" %x ", 10);
// 	ft_printf(" %x ", 11);
// 	ft_printf(" %x ", 15);
// 	ft_printf(" %x ", 16);
// 	ft_printf(" %x ", 17);
// 	ft_printf(" %x ", 99);
// 	ft_printf(" %x ", 100);
// 	ft_printf(" %x ", 101);
// 	ft_printf(" %x ", -9);
// 	ft_printf(" %x ", -10);
// 	ft_printf(" %x ", -11);
// 	ft_printf(" %x ", -14);
// 	ft_printf(" %x ", -15);
// 	ft_printf(" %x ", -16);
// 	ft_printf(" %x ", -99);
// 	ft_printf(" %x ", -100);
// 	ft_printf(" %x ", -101);
// 	ft_printf(" %x ", INT_MAX);
// 	ft_printf(" %x ", INT_MIN);
// 	ft_printf(" %x ", LONG_MAX);
// 	ft_printf(" %x ", LONG_MIN);
// 	ft_printf(" %x ", UINT_MAX);
// 	ft_printf(" %x ", ULONG_MAX);
// 	ft_printf(" %x ", 9223372036854775807LL);
// 	ft_printf(" %x %x %x %x %x %x %x", INT_MAX, INT_MIN, LONG_MAX, LONG_MIN, ULONG_MAX, 0, -42);
// 	ft_printf(" %x ", 42);
// 	ft_printf(" %x ", -42);
// 	return (0);
// }