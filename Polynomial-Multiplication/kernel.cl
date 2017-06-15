void kernel poly_multiply(global const int *a, global const int *b, global int *c)
{
    size_t i = get_global_id(0);
    c[i] = a[i] * b[i];
}