#!/bin/sh

script_path=${1:?}
directory="$(dirname "$script_path")"

cd "$directory" || exit

pip install -r requirements.txt
exec python "$script_path"
