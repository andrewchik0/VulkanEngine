# Path variables
glslc_path=
project_path=

cd "$project_path/shaders"

recurse() {
  path=$1
  if [ -d "$path" ] ; then
     for i in "$path/"*
     do
        recurse "$i"
     done
  elif [ -f "$path" ] ; then
    filename=$(basename "$path")
    if [ "${filename: -5}" == ".frag" \
      -o "${filename: -5}" == ".vert" \
      -o "${filename: -5}" == ".comp" ]
    then
      $glslc_path $path -o $filename.spv
    fi
  fi
}
recurse $project_path/shaders
