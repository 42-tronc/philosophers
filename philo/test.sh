#!/bin/bash

cyan="\e[36m"
bold="\e[1m"
unbold="\e[21m"
reset="\e[0m"
# set -e # Exit immediately if a command exits with a non-zero status


# Function to build the project
build_project() {
    make
}

# Function to run Valgrind helgrind test
run_helgrind_test() {
    valgrind --tool=helgrind --track-lockorders=yes ./philo "$@"
}

# Function to run Valgrind memcheck test
run_memcheck_test() {
    valgrind_output=$(valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all ./philo "$@" 2>&1)
    if echo "$valgrind_output" | grep -E "definitely lost|indirectly lost"; then
        echo "LEAKS ❌"
        echo "$valgrind_output"
    else
        echo "OK ✅"
    fi
}

# Build the project
build_project

# Test multiple sets of arguments
# declare -a test_args=("6 500 200 200 10")
# declare -a test_args=("3 610 200 200 5")
# declare -a test_args=("20 1100 500 500 20")
# declare -a test_args=("5 300 200 200 2")
declare -a test_args=("10 500 200 200 10" "5 300 100 100 5" "20 1100 500 500 20" "3 900 300 300 10" "5 300 200 200 2")

for args in "${test_args[@]}"; do
    echo -e "\n\n${cyan}Testing with arguments: ${bold}$args$reset"

    # Run Valgrind helgrind test
    run_helgrind_test $args

    # Run Valgrind memcheck test with specific options
    run_memcheck_test $args
done
