/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschyns <mano.schyns@learner.42.tech>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 13:31:30 by mschyns           #+#    #+#             */
/*   Updated: 2026/07/22 13:31:30 by mschyns          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// Return 0 en cas d'erreur
// Set la value dans number
// Si la valeur == 0 -> Considéré comme une erreur
static int	ft_atoi(char *str, int *number)
{
	long	result;
	int		i;

	i = 0;
	result = 0;
	if (str[i] == '\0')
		return (0);
	while (str[i] != '\0')
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		result = result * 10 + str[i] - '0';
		if (result > INT_MAX)
			return (0);
		i ++;
	}
	*number = (int) result;
	return (1);
}

// Return Null Si tous les arguments ont pu etre recupérés.
// Return un message si non
static char	*verification(char **argv, t_args *args)
{
	char	*message;

	message = NULL;
	if (!ft_atoi(argv[0], &(args->number_of_coders)))
		message = "Invalid number of coders\n";
	else if (args->number_of_coders == 0)
		message = "Number of coder must be positive\n";
	else if (!ft_atoi(argv[1], &(args->time_to_burnout)))
		message = "Invalid time to burnout\n";
	else if (!ft_atoi(argv[2], &(args->time_to_compile)))
		message = "Invalid time to compile\n";
	else if (!ft_atoi(argv[3], &(args->time_to_debug)))
		message = "Invalid time to debug\n";
	else if (!ft_atoi(argv[4], &(args->time_to_refactor)))
		message = "Invalid time to refactor\n";
	else if (!ft_atoi(argv[5], &(args->number_of_compiles_required)))
		message = "Invalid number of compiles required\n";
	else if (!ft_atoi(argv[6], &(args->dongle_cooldown)))
		message = "Invalid dongle cooldown\n";
	else if (strcmp(argv[7], "fifo") != 0 && strcmp(argv[7], "edf") != 0)
		message = "Invalid scheduler\n";
	else
		args->scheduler = argv[7];
	return (message);
}

// Retorune les arguments qui ont été parsés.
// Afiche un message d'erreur en cas d'argument invalide
t_args	parse_args(int argc, char **argv)
{
	t_args	args;
	char	*output_verif;

	args.parsing_flag = 0;
	if (argc != N_ARGS)
	{
		printf("%s\n", "Arguments are missing.");
		return (args);
	}
	output_verif = verification(argv, &args);
	if (output_verif != NULL)
	{
		printf("%s", output_verif);
		return (args);
	}
	args.parsing_flag = 1;
	return (args);
}
