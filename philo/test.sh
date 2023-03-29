#!/bin/bash
# by Maplepy

# # Set the duration of the test in seconds
# DURATION=60
# # Wait for the program to exit or for the duration of the test
# sleep $DURATION

orange="\e[38;5;208m"
light_orange="\e[38;5;214m"
yellow="\e[38;5;220m"

fg_black="\e[30m"
fg_red="\e[31m"
fg_green="\e[32m"
fg_yellow="\e[33m"
fg_blue="\e[34m"
fg_magenta="\e[35m"
fg_cyan="\e[36m"
fg_light_gray="\e[37m"
fg_default="\e[39m"
fg_dark_gray="\e[90m"
fg_light_red="\e[91m"
fg_light_green="\e[92m"
fg_light_yellow="\e[93m"
fg_light_blue="\e[94m"
fg_light_magenta="\e[95m"
fg_light_cyan="\e[96m"
fg_white="\e[97m"

bg_black="\e[40m"
bg_red="\e[41m"
bg_green="\e[42m"
bg_yellow="\e[43m"
bg_blue="\e[44m"
bg_magenta="\e[45m"
bg_cyan="\e[46m"
bg_light_gray="\e[47m"
bg_default="\e[49m"
bg_dark_gray="\e[100m"
bg_light_red="\e[101m"
bg_light_green="\e[102m"
bg_light_yellow="\e[103m"
bg_light_blue="\e[104m"
bg_light_magenta="\e[105m"
bg_light_cyan="\e[106m"
bg_white="\e[107m"

bold="\e[1m"
dim="\e[2m"
italic="\e[3m"
underline="\e[4m"
blink="\e[5m"
reverse="\e[7m"
hidden="\e[8m"
striked="\e[9m"
reset="\e[0m"

echo -e "\ec${fg_green}
		8888888b.  888      d8b 888
		888   Y88b 888      Y8P 888
		888    888 888          888
		888   d88P 88888b.  888 888  .d88b.
		8888888P\"  888 \"88b 888 888 d88\"\"88b
		888        888  888 888 888 888  888
		888        888  888 888 888 Y88..88P
		888        888  888 888 888  \"Y88P\"
		by Maplepy
		${reset}
"

print_info() {
	printf "\n\n$orange%*s" "${COLUMNS:-$(tput cols)}" '' | tr ' ' -
	# echo -e "\n\t${orange}------------------------${reset}"
	echo -e "${light_orange}●◯${reset} ${bold}$1${reset}\n"
}

print_test() {
	# printf "\n$orange%*s" "${COLUMNS:-$(tput cols)}" '' | tr ' ' -
	# echo -e "\n\t${orange}------------------------${reset}"
	echo -e  "\n\t${light_orange}${bold}○${reset} Running test with: ${yellow}${bold}'$1'${reset}"
}

print_success() {
	echo -e "${fg_green}${bold}\t[SUCCESS]${reset} ${bold}$1${reset}"
}

print_error() {
	echo -e "${fg_red}${bold}✕\t[ERROR]${reset} ${bold}$1${reset}"
}

run_leak_test() {
	print_test "$1 $2 $3 $4 $5"
	valgrind_output=$(valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./philo "$1" "$2" "$3" "$4" "$5" 2>&1)

	if echo "$valgrind_output" | grep -q "ERROR SUMMARY: 0 errors from 0 contexts"; then
		if echo "$valgrind_output" | grep -q "All heap blocks were freed -- no leaks are possible"; then
			print_success "No memory leaks detected"
		else
			print_error "Test failed: memory leaks detected"
		fi
	else
		print_error "Test failed: program crashed or produced memory errors"
		sleep 2
		echo "$valgrind_output"
		exit 1
	fi
}

run_unit_test() {
	# nb_philo death_time eat_time sleep_time (meal_required) timeout
	start_time=$(date +%s.%N)
	nb_philo=$1
	death_time=$2
	eat_time=$3
	sleep_time=$4

	# no meal count required
	if [[ -z "$6" ]]; then
		timeout=$5
		print_test "$nb_philo $death_time $eat_time $sleep_time"
		./philo "$nb_philo" "$death_time" "$eat_time" "$sleep_time" 2>&1 | tail -n 1 &
	# meal count required
	else
		meal_required=$5
		timeout=$6
		print_test "$nb_philo $death_time $eat_time $sleep_time $meal_required"
		./philo "$nb_philo" "$death_time" "$eat_time" "$sleep_time" "$meal_required" 2>&1 | tail -n 1 &
	fi

	# get runtime
	pid=$!
	# echo "pid = $pid"
	wait "$pid"
	end_time=$(date +%s.%N)
	runtime=$(echo "($end_time - $start_time) * 1000" | bc -l | cut -d '.' -f 1)
	# echo -e "${bg_cyan}-> Runtime: $runtime ms${reset}"

	# if no meal count required and should die
	if [[ -z "$6" ]]; then
		# sleep 30
		print_success "Program kept running ($runtime ms)"
		kill "$pid"
	# no meal count required and should live
	# elif [[ "$5" == "-1" ]]; then
	# 	echo "The two strings are the same"
	# 	sleep 30
	# 	print_success "Program kept running ($runtime ms)"
	# 	kill "$pid"
	# meal count required
	else
		# runtime too long
		if [ "$runtime" -gt "$6" ]; then
			echo "Program took too long to run"
			print_error "Test failed: program did not exit after $6 ms"
			kill "$pid"
		else
			print_success "Program exited after ${runtime}ms (timeout ${6}ms)"
		fi
	fi
}

# compile the program
print_info "Compiling the program"
make re

# memory leak check tests
# print_info "Launching memory leak tests..."

# run_leak_test 2 800 200 200 10
# run_leak_test 4 410 200 200 25
# run_leak_test 5 800 200 200 25
# run_leak_test 4 200 200 200 25
# run_leak_test
# run_leak_test 2
# run_leak_test -4 200 200 200 25
# run_leak_test 2 100
# run_leak_test 2 100 20
# run_leak_test 2 100 20 -20
# run_leak_test 2 100 20 20 -5

# philo death tests
print_info "Launching death tests..."
# nb_philo death_time eat_time sleep_time (meal_required) timeout
run_unit_test 1	200	50	50	50	210
run_unit_test 1	200	50	50		-1
run_unit_test 4	310	200	100		-1

print_info "Launching infinite-ish tests..."
run_unit_test 5 800 200 200 -1


# run_unit_test 1 100 100 100
# run_unit_test 2 1 100 100
# run_unit_test 2 2 100 1000
# run_unit_test 3 3 100 2000
# run_unit_test 4 2 100 2000
# run_unit_test 5 1 100 2000
# run_unit_test 6 5 100 5000
# run_unit_test 7 4 100 4000
# run_unit_test 8 3 100 2000
# run_unit_test 9 2 50 2000
# run_unit_test

# # Set the duration of the test in seconds
# DURATION=60

# # Run the program in the background
# ./philo &

# # Save the PID of the program
# PID=$!

# # Wait for the program to exit or for the duration of the test
# sleep $DURATION

# # Check if the program is still running
# if ps -p $PID >/dev/null; then
# 	# If the program is still running, kill it and print an error message
# 	kill $PID
# 	echo "Test failed: Program did not exit within $DURATION seconds"
# 	exit 1
# else
# 	# If the program has exited, print a success message
# 	echo "Test passed: Program exited within $DURATION seconds"
# 	exit 0
# fi
