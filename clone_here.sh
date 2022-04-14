#!/usr/bin/env bash
git clone https://derobins@github.com/derobins/hdf5.git .
git remote add canonical https://derobins@github.com/HDFGroup/hdf5.git
git remote set-url canonical --push "DO NOT PUSH TO CANONICAL"
git fetch canonical
git checkout canonical/develop
