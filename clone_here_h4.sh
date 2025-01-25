#!/usr/bin/env bash
git clone https://derobins@github.com/derobins/hdf4.git .
git remote add canonical https://derobins@github.com/HDFGroup/hdf4.git
git remote set-url canonical --push "DO NOT PUSH TO CANONICAL"
git fetch canonical
git checkout canonical/master
git config user.email "dana.e.robinson@gmail.com"
