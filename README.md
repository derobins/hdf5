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
