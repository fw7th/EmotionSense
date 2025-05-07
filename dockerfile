FROM ubuntu:22.04

# Set environment variables to avoid some tzdata prompts
ENV DEBIAN_FRONTEND=noninteractive

# Install system packages
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    wget \
    unzip \
    pkg-config \
    libopencv-dev \
    libeigen3-dev \
    && rm -rf /var/lib/apt/lists/*

# Create app directory inside container
WORKDIR /app
