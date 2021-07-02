# Open3D GPU docker CI

```bash
# First, cd to the top Open3D directory

# 1. Build image
docker build -t open3d-ci:latest -f docker/Dockerfile .

# 2. Run in detached mode
# You'll need to mkdir -p ~/.docker_ccache in host
docker run -d -it --rm --gpus all -v ~/.docker_ccache:/root/.cache/ccache --name ci open3d-ci:latest
# Otherwise, you may also run without ccache
docker run -d -it --rm --gpus all -v --name ci open3d-ci:latest

# 3. Attach to container
docker exec -it ci /bin/bash

# 4. Build and test
# Now you should be in /root/Open3D inside docker
./docker/build_and_test.sh

# 5. Stop container
docker stop ci

# Extra: sun 2&3 together
docker run -d -it --rm --gpus all -v ~/.docker_ccache:/root/.cache/ccache --name ci open3d-ci:latest && docker exec -it ci /bin/bash

# Extra: attach to container and build Open3D
docker exec -it ci /root/Open3D/docker/build_and_test.sh

# Extra: run in detached mode, attach to container and build Open3D
docker run -d -it --rm --gpus all -v ~/.docker_ccache:/root/.cache/ccache --name ci open3d-ci:latest && docker exec -it ci /root/Open3D/docker/build_and_test.sh
docker run -d -it --rm --gpus all -v ~/.docker_ccache:/root/.cache/ccache --name ci open3d-ci:latest && docker exec -it ci /root/Open3D/docker/build_and_test.sh && docker stop ci

# Extra: attach to container and monitor ccache
docker exec -it ci watch -n 1 ccache -s

# Extra: debugging, run command immediately
docker run --rm --gpus all open3d-ci:latest nvidia-smi
docker run --rm --gpus all open3d-ci:latest ccache -s

# Extra: sanity checks
docker run --rm --gpus all nvidia/cuda:11.0.3-cudnn8-devel-ubuntu18.04 nvidia-smi
```
