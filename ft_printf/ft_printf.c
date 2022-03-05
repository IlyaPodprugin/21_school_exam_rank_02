#include "./ft_printf.h"

typedef struct s_flags
{
	int	width;
	int	prec;
}	t_flags;

int	ft_strlen(char *str)
{
	int	len = 0;
	while (*str++)
		len++;
	return (len);
}

int	ft_print_str(char *str, t_flags *flags)
{
	int	len;
	int	spaces;

	len = 0;
	spaces = 0;
	if (flags->prec == 0 || (!str && flags->prec < 6 && flags->prec >= 0))
		return (0);
	if (!str)
		str = "(null)";
	if (flags->prec > 0)
	{
		if (flags->width != 0 && flags->prec <= flags->width)
		{
			spaces = flags->width - flags->prec;
			while (spaces)
			{
				len += write(1, &" ", 1);
				spaces--;
			}
		}
		
		while (*str && flags->prec > 0)
		{
			len += write(1, str++, 1);
			flags->prec--;
		}
	}
	else if (flags->width != 0)
	{
		int	str_len = ft_strlen(str);
		while (str_len < flags->width)
		{
			len += write(1, &" ", 1);
			flags->width--;
		}
		while (*str)
			len += write(1, str++, 1);
	}
	else
		while (*str)
			len += write(1, str++, 1);
	
	return (len);
}

int	ft_len(int n)
{
	int	digits = 0;
	if (n == 0)
		digits = 1;
	else
	{
		while (n)
		{
			n /= 10;
			digits++;
		}
	}
	return (digits);
}

char	*recursive_fill_result(char *str, long int num)
{
	if (num / 10 > 0)
		str = recursive_fill_result(str, num / 10);
	*str = num % 10 + 48;
	return (++str);
}

char	*ft_itoa(int num)
{
	int	is_neg = 0;
	int	digits = 0;
	long int	n;
	char	*res;

	if (num < 0)
	{
		is_neg++;
		digits++;
		n = (long)num * -1;
	}
	else
		n = num;
	digits += ft_len(n);
	res = (char *)malloc(digits * sizeof(char) + 1);
	if (!res)
		return (0);
	if (num < 0)
		*res = '-';
	recursive_fill_result(&res[is_neg], n);
	res[digits] = '\0';
	return (res);
}

int	ft_print_digit(int num, t_flags *flags)
{
	int	len = 0;

	char	*res = ft_itoa(num);
	len = ft_print_str(res, flags);
	free(res);
	return (len);
}

int	count_size(unsigned int digit)
{
	int	counter = 0;

	while (digit != 0)
	{
		digit /= 16;
		counter++;
	}
	return (counter);
}

void	fill_result(char *res, int size, unsigned int digit)
{
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

int	ft_print_hex(unsigned int digit, t_flags *flags)
{
	int	len = 0;
	int	size = count_size(digit);
	if (digit != 0)
	{
		char	*res = (char *)malloc(sizeof(char) * (size + 1));
		if (!res)
			return (0);
		res[size] = '\0';
		fill_result(res, size, digit);
		len = ft_print_str(res, flags);
		free(res);
	}
	else
		len = ft_print_str("0", flags);
	return (len);

}

int	ft_define_conv(const char *str, va_list args, int i, t_flags *flags)
{
	int	len;

	len = 0;
	if (str[i] == 's')
		len += ft_print_str(va_arg(args, char *), flags);
	else if (str[i] == 'd')
		len = ft_print_digit(va_arg(args, int), flags);
	else if (str[i] == 'x')
		len = ft_print_hex(va_arg(args, unsigned int), flags);

	return (len);
}

int	ft_printf(const char *str, ...)
{
	t_flags	flags;
	va_list	args;
	int		len;
	int		i;
	
	len = 0;
	i = 0;
	va_start(args, str);
	while (str[i])
	{
		flags.prec = -1;
		flags.width = 0;
		if (str[i] == '%')
		{
			i++;
			if (str[i] >= 48 && str[i] <= 57)
			{
				flags.width = str[i] - 48;
				i++;
				while (str[i] >= 48 && str[i] <= 57)
				{
					flags.width = flags.width * 10 + (str[i] - 48);
					i++;
				}
			}
			if (str[i] == '.')
			{
				i++;
				if (str[i] >= 48 && str[i] <= 57)
				{
					flags.prec = str[i] - 48;
					i++;
				}
				else
					flags.prec = 0;
				while (str[i] >= 48 && str[i] <= 57)
				{
					flags.prec = flags.prec * 10 + (str[i] - 48);
					i++;
				}
			}
			len += ft_define_conv(str, args, i, &flags);
		}
		else
			len += write(1, &str[i], 1);
		i++;
	}
	va_end(args);
	return (len);
}

// int	main(void)
// {
// 	int	len = 0;
// 	char	*null_str = NULL;

// 	len = ft_printf("[%s]\n", null_str);
// 	printf("%d\n", len);
// 	return (0);
// }
