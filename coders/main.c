#include "codexion.h"

int	main(int argc, char **argv)
{
	t_args	args;

	args = parse_args(argc - 1, argv + 1);
	if (!args.parsing_flag)
		return (1);
	
	// a supp
	
	printf("%d\n", args.parsing_flag);
	printf("%d\n", args.number_of_coders);
	printf("%d\n", args.number_of_compiles_required);
	printf("%d\n", args.time_to_burnout);
	printf("%d\n", args.time_to_compile);
	printf("%d\n", args.time_to_debug);
	printf("%d\n", args.time_to_refactor);
	printf("%d\n", args.dongle_cooldown);
	printf("%s\n", args.scheduler);
}
