#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdlib.h>

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

int	count_size(int digit)
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
		char	*res = (char *)malloc(size * sizeof(char) + 1);
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
	flags.prec = -1;
	flags.width = 0;
	va_start(args, str);
	while (str[i])
	{
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
				flags.prec = str[i] - 48;
				i++;
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

int	main(void)
{
	ft_printf("Simple test\n");
	ft_printf("");
	ft_printf("--Format---");
	ft_printf("\n");
	ft_printf("%d\n", 0);
	ft_printf("%d\n", 42);
	ft_printf("%d\n", 1);
	ft_printf("%d\n", 5454);
	ft_printf("%d\n", (int)2147483647);
	ft_printf("%d\n", (int)2147483648);
	ft_printf("%d\n", (int)-2147483648);
	ft_printf("%d\n", (int)-2147483649);
	printf("-----------orig-----------\n");
	printf("%d\n", (int)2147483647);
	printf("%d\n", (int)2147483648);
	printf("%d\n", (int)-2147483648);
	printf("%d\n", (int)-2147483649);
	ft_printf("\n");
	ft_printf("%x\n", 0);
	ft_printf("%x\n", 42);
	ft_printf("%x\n", 1);
	ft_printf("%x\n", 5454);
	ft_printf("%x\n", (int)2147483647);
	ft_printf("%x\n", (int)2147483648);
	ft_printf("%x\n", (int)-2147483648);
	ft_printf("%x\n", (int)-2147483649);
	ft_printf("%s\n", "");
	ft_printf("%s\n", "toto");
	ft_printf("%s\n", "wiurwuyrhwrywuier");
	ft_printf("%s\n", NULL);
	ft_printf("-%s-%s-%s-%s-\n", "", "toto", "wiurwuyrhwrywuier", NULL);
	ft_printf("\n--Mixed---\n");
	ft_printf("%d%x%d%x%d%x%d%x\n", 0, 0, 42, 42, 2147483647, 2147483647, (int)-2147483648, (int)-2147483648);
	ft_printf("-%d-%x-%d-%x-%d-%x-%d-%x-\n", 0, 0, 42, 42, 2147483647, 2147483647, (int)-2147483648, (int)-2147483648);
	ft_printf("\n");
	ft_printf("%s%s%s%s\n", "", "toto", "wiurwuyrhwrywuier", NULL);
	ft_printf("-%s-%s-%s-%s-\n", "", "toto", "wiurwuyrhwrywuier", NULL);
	ft_printf("--1 Flag--\n");
	ft_printf("d0w %0d %0d %0d %0d %0d %0d %0d %0d\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
	ft_printf("d4w %4d %4d %4d %4d %4d %4d %4d %4d\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
	ft_printf("d10w %10d %10d %10d %10d %10d %10d %10d %10d\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
	ft_printf("d0p %.0d %.0d %.0d %.0d %.0d %.0d %.0d %.0d\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
	ft_printf("d4p %.4d %.4d %.4d %.4d %.4d %.4d %.4d %.4d\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
	ft_printf("d10p %.10d %.10d %.10d %.10d %.10d %.10d %.10d %.10d\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
	ft_printf("x0w %0x %0x %0x %0x %0x %0x %0x %0x\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
	ft_printf("x4w %4x %4x %4x %4x %4x %4x %4x %4x\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
	ft_printf("x10w %10x %10x %10x %10x %10x %10x %10x %10x\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
	ft_printf("x0p %.0x %.0x %.0x %.0x %.0x %.0x %.0x %.0x\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
	ft_printf("x4p %.4x %.4x %.4x %.4x %.4x %.4x %.4x %.4x\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
	ft_printf("x10p %.10x %.10x %.10x %.10x %.10x %.10x %.10x %.10x\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
	ft_printf("s0p ~%.0s` ~%.0s` ~%.0s` ~%.0s` ~%.0s`\n", "", "toto", "0123456789", "tjehurthteutuiehteute", NULL);
	ft_printf("s4w ~%4s` ~%4s` ~%4s` ~%4s` ~%4s`\n", "", "toto", "0123456789", "tjehurthteutuiehteute", NULL);
	ft_printf("s4p ~%.4s` ~%.4s` ~%.4s` ~%.4s` ~%.4s`\n", "", "toto", "0123456789", "tjehurthteutuiehteute", NULL);
	ft_printf("s10w ~%10s` ~%10s` ~%10s` ~%10s` ~%10s`\n", "", "toto", "0123456789", "tjehurthteutuiehteute", NULL);
	ft_printf("s10p ~%.10s` ~%.10s` ~%.10s` ~%.10s` ~%.10s`\n", "", "toto", "0123456789", "tjehurthteutuiehteute", NULL);
	ft_printf("--2 Flags--\n");
	ft_printf("d0w0p %0.0d %0.0d %0.0d %0.0d %0.0d %0.0d %0.0d %0.0d\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
	ft_printf("x0w0p %0.0x %0.0x %0.0x %0.0x %0.0x %0.0x %0.0x %0.0x\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
	ft_printf("--Precision--\n");
	ft_printf("d0w4p %0.4d %0.4d %0.4d %0.4d %0.4d %0.4d %0.4d %0.4d\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
	ft_printf("d0w10p %0.10d %0.10d %0.10d %0.10d %0.10d %0.10d %0.10d %0.10d\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
	ft_printf("x0w4p %0.4x %0.4x %0.4x %0.4x %0.4x %0.4x %0.4x %0.4x\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
	ft_printf("x0w10p %0.10x %0.10x %0.10x %0.10x %0.10x %0.10x %0.10x %0.10x\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
	ft_printf("--Width--\n");
	ft_printf("d4w0p %4.0d %4.0d %4.0d %4.0d %4.0d %4.0d %4.0d %4.0d\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
	ft_printf("d10w0p %10.0d %10.0d %10.0d %10.0d %10.0d %10.0d %10.0d %10.0d\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
	ft_printf("x4w0p %4.0x %4.0x %4.0x %4.0x %4.0x %4.0x %4.0x %4.0x\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
	ft_printf("x10w0p %10.0x %10.0x %10.0x %10.0x %10.0x %10.0x %10.0x %10.0x\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
	ft_printf("s4w0p ~%4.s` ~%4.s` ~%4.s` ~%4.s` ~%4.s`\n", "", "toto", "0123456789", "tjehurthteutuiehteute", NULL);
	ft_printf("s10w0p ~%10.0s` ~%10.0s` ~%10.0s` ~%10.0s` ~%10.0s`\n", "", "toto", "0123456789", "tjehurthteutuiehteute", NULL);
	ft_printf("--Width and Precision--\n");
	ft_printf("d10w4p %10.4d %10.4d %10.4d %10.4d %10.4d %10.4d %10.4d %10.4d\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
	ft_printf("d10w10p %10.10d %10.10d %10.10d %10.10d %10.10d %10.10d %10.10d %10.10d\n", 0, 1, 42, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
	ft_printf("d4w4p %4.4d %4.4d %4.4d %4.4d %4.4d %4.4d %4.4d %4.4d\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
	ft_printf("d4w10p %4.10d %4.10d %4.10d %4.10d %4.10d %4.10d %4.10d %4.10d\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
	ft_printf("x10w4p %10.4x %10.4x %10.4x %10.4x %10.4x %10.4x %10.4x %10.4x\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
	ft_printf("x10w10p %10.10x %10.10x %10.10x %10.10x %10.10x %10.10x %10.10x %10.10x\n", 0, 1, 42, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
	ft_printf("x4w4p %4.4x %4.4x %4.4x %4.4x %4.4x %4.4x %4.4x %4.4x\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
	ft_printf("x4w10p %4.10x %4.10x %4.10x %4.10x %4.10x %4.10x %4.10x %4.10x\n", 0, 42, 1, 4554, 2147483647, (int)2147483648, (int)-2147483648, (int)-2147483649);
	ft_printf("s10w4p ~%10.4s` ~%10.4s` ~%10.4s` ~%10.4s` ~%10.4s`\n", "", "toto", "0123456789", "tjehurthteutuiehteute", NULL);
	ft_printf("s10w10p ~%10.10s` ~%10.10s` ~%10.10s` ~%10.10s` ~%10.10s`\n", "", "toto", "0123456789", "tjehurthteutuiehteute", NULL);
	ft_printf("s4w4p ~%4.4s` ~%4.4s` ~%4.4s` ~%4.4s` ~%4.4s`\n", "", "toto", "0123456789", "tjehurthteutuiehteute", NULL);
	ft_printf("s4w10p ~%10.10s` ~%10.10s` ~%10.10s` ~%10.10s` ~%10.10s`\n", "", "toto", "0123456789", "tjehurthteutuiehteute", NULL);
	return (0);
}