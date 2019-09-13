if [[ ! -d ../cs107e.github.io ]]; then
  echo "This script should be run in a directory that its parent directory contains the cs107e.github.io repository"
  exit 1
fi

parent_dir=$(readlink -f ..)

export PATH="$parent_dir/cs107e.github.io/cs107e/bin:$PATH"
export CS107E="$parent_dir/cs107e.github.io/cs107e"
