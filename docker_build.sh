CONTAINER_PROJ_PATH="/root/tools"
ROOT_PROJ_PATH=$(cd $(dirname $0) && pwd)
docker run -dti --name devkafka -v ${ROOT_PROJ_PATH}:${CONTAINER_PROJ_PATH} --net=host kafkabridge