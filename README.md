# Personal branch strategy

* My personal fork does not duplicate anything in `HDFGroup/hdf5` - features and bugfixes only!
* `HEAD` points to this branch, named `NOPE`, which is essentially empty
* I clone via the `clone_here` script included in this file:
    * Clones my personal fork
    * Adds `HDFGroup/hdf5` as a remote named `canonical`
    * Sets the `canonical` push URL to nonsense to avoid creating branches there
    * Fetches `canonical` and checks out the `develop` branch
* When starting a feature branch, I branch off of the development/feature branches in `HDFGroup/hdf5`
* I try to prefix features and fixes with something useful, like "10" for merges to `hdf5_1_10`
* Anything unprefixed was probably spun off of `develop`

## Why do I do this?

* Syncing personal copies of `HDFGroup/hdf5` is a waste of time
    * Also, doing this poorly can generate ridiculous log entries showing every merge commit made in your personal copy
* No wading through canonical HDF5 stuff to find my personal branches
* Easily spin branches off of `HDFGroup/hdf5` and move code around
* No name clash - `git checkout develop` resolves to `canonical/develop`
* Works like a personal extension to `HDFGroup/hdf5`
