#!/usr/bin/env bash

matrix=()

matrix_data=$(cat $1)
num_of_rows=0

for row in $matrix_data
do
	let num_of_rows++
done

let num_of_columns=num_of_rows+1

i=0
for row in $matrix_data
do
	j=0
	IFS=","
	let i++
	for value in $row
	do
		let j++
		matrix[((num_of_columns*i+j))]=$value
	done
	IFS='/n'
done

swap_strings() {  # indexes $1 and $2
	for (( j=1; j<=num_of_columns; j++ )); do
        	tmp=${matrix[((num_of_columns*$1+j))]}
		matrix[((num_of_columns*$1+j))]=${matrix[((num_of_columns*$2+j))]}
		matrix[((num_of_columns*$2+j))]=$tmp
	done
}

print_matrix() {  # for debug
	for (( i=1; i<=num_of_rows; i++ )); do
        	for (( j=1; j<=num_of_columns; j++ )); do
        	        echo ${matrix[((num_of_columns*i+j))]}
        	done
        	echo
	done
}

add_lc() {  # add linear combination of $2 str with $3 coef to $1 str
	for (( j=1; j<=num_of_columns; j++ )); do
		matrix[((num_of_columns*$1+j))]=$(echo "${matrix[((num_of_columns*$2+j))]}*$3+${matrix[((num_of_columns*$1+j))]}" | bc -l)
	done
}

normalise() {  # makes the $1-th step of gauss-algorithm
	for (( i=$1; i<=num_of_rows; i++ )); do
		if echo "${matrix[num_of_columns*i+$1]}!=0" | bc >> /dev/null
		then
			break
		fi
	done
	swap_strings $1 $i
	for (( i=num_of_columns; i>=1; i-- )); do
		matrix[((num_of_columns*$1+i))]=$(echo "${matrix[((num_of_columns*$1+$i))]}/${matrix[((num_of_columns*$1+$1))]}" | bc -l)
	done
	for (( i=1; i<=num_of_rows; i++ )); do
		if [[ $i -ne $1 ]]
		then
			coef=$(echo "-1*${matrix[num_of_columns*i+$1]}/${matrix[num_of_columns*$1+$1]}" | bc -l)
			add_lc $i $1 $coef
		fi
	done
}

for (( k=1; k<=num_of_rows; k++ )); do
	normalise $k
done
for (( k=1; k<=num_of_rows; k++ )); do
	echo "${matrix[num_of_columns*(k+1)]}" | bc -l
done
