names="actor director event game input physics render"
for name in $names
do
    sed -i "s/${name}.h/gamo\/${name}\/${name}.h/g" `grep "${name}.h" -rl`
done
