Algorithm SelectionSort(A, n)
{
	for i := 1 to n do {
		j := i;								// initialize j to be i
		for k := i + 1 to n do 		// find the smallest in A[i + 1 : n]
			if (A[k] < A[j]) then j := k;	//found, record the index
		t := A[i]; A[i] := A[j]; A[j] := t;	// swap A[i] and A[j]
	}
}

Algorithm InsertionSort(A, n)
{
	for j := 2 to n do {	// assume A[1 : j - 1] already sorted
		item := A[j];		// move A[j] to its proper place
		i := j - 1;			// initialize i to be j - 1
		while ((i >= 1) and (item < A[i])) do {
								// find i such that A[i] <= A[j]
			A[i + 1] := A[i];	// move A[i] up by one position
			i := i - 1;
		}
		A[i + 1] := item;		// move A[j] to A[i + 1]
	}
}

Algorithm BubbleSort(A, n)
{
	for i := 1 to n - 1 do {		// find the smallest item for A[i]
		for j := n to i + 1 step -1 do {
			if (A[j] < A[j - 1]) {	// swap A[j] and A[j - 1]
				t := A[j]; A[j] := A[j - 1]; A[j - 1] := t;
			}
		}
	}
}

Algorithm ShakerSort(A, n)
{
	l := 1; r := n;
	while l <= r do {
		for j := r to l + 1 step -1 do {// element exchange from r to l
			if (A[j] < A[j - 1]) {		// swap A[j] and A[j - 1]
				t := A[j]; A[j] := A[j - 1]; A[j - 1] := t;
			}
		}
		l := l + 1;
		for j := l to r - 1 do {		// element exchange from l to r
			if (A[j] > A[j + 1]) {		// swap A[j] and A[j + 1]
				t := A[j]; A[j] := A[j + 1]; A[j + 1] := t;
			}
		}
		r := r - 1;
	}
}