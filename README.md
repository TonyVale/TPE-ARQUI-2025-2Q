##SO made in assembler + C with barebonex64.

#Project description

SO based on X64Bare-Bones. 
Console commands.
Self-developed drivers. 
Tron game. 

#RUN

docker run -d -v ${PWD}:/root --security-opt seccomp:unconfined -it --name TPE  agodio/itba-so:2.0

docker start TPE

docker exec -ti TPE bash

./run.sh
