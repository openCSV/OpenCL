void kernel poly_multiply(global const int *a, global const int *b, global int *c, const int n)
{
    int i = get_global_id(0);
    c[i] = 0;
    for(int j=0; j<n; j++)
        if(i >= j && i-j < n)
            c[i] += a[j] * b[i-j];
}