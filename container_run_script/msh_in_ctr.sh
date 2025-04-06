#!/bin/bash

MY_TEST=my_test
PATH_ROOT=$2
RED="\033[0;31m"
RESET="\033[0;0m"
CTR_MSH_PATH=/home/minishell
NUM_ARGS=0


if [[ $# -lt 2 && "$1" != "--stop" ]]; then
    echo ${RED}"Use: $0 <-run/-stop> <path minishell root>" ${RESET}
    exit 1
fi
#echo "checking if container allreally exist..."
if [[ $(docker ps -a | grep -w my_test | awk '{print $2}' | grep -w ubuntu | wc -l) -eq 1 \
	&& "$1" = "--run" ]]; then
    #echo "the container my_test ever exist!"
	docker cp ${PATH_ROOT} ${MY_TEST}:${CTR_MSH_PATH}
	docker exec -it ${MY_TEST} make re -C ${CTR_MSH_PATH}/libft
	docker exec -it ${MY_TEST} make re -C ${CTR_MSH_PATH}
	docker exec -it ${MY_TEST} ${CTR_MSH_PATH}/minishell
elif [ "$1" = "--run" ]; then
    echo "Criating container my_test..."
	#open -a docker
    docker pull ubuntu
    docker run -dit --rm --name "my_test" ubuntu
    docker exec -it my_test apt update
	#docker exec -it my_test apt install gcc -y
	docker exec -it my_test apt install libreadline-dev -y
	#docker exec -it my_test apt install make -y
	docker exec -it my_test apt install build-essential -y
	docker cp ${PATH_ROOT} ${MY_TEST}:${CTR_MSH_PATH}
	docker exec -it ${MY_TEST} make re -C ${CTR_MSH_PATH}/libft
	docker exec -it ${MY_TEST} make re -C ${CTR_MSH_PATH}
	docker exec -it ${MY_TEST} ${CTR_MSH_PATH}/minishell
fi

if [[ $(docker ps -a | grep -w my_test | awk '{print $2}' | grep -w ubuntu | wc -l) -eq 1 \
	&& "$1" = "--stop" ]]; then
	docker stop ${MY_TEST}
fi