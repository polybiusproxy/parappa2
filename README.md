# PaRappa the Rapper 2 Decompilation

### Setup

1. Create a folder named `iso` in the root of the repo.
2. Extract `SCPS_150.17` from an ISO of the July 12th prototype and place it in said `iso` folder.
3. `./configure.py`
4. Execute the following command: `grep -rlE '%(gp_rel)' asm/nonmatchings/* | xargs sed -i -E -s 's/%(gp_rel)\(([^)]+)\)\(\$28\)/\2/' 2>/dev/null || true`
5. `ninja`