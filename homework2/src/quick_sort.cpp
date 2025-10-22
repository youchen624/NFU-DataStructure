void term_quick_sort(int arr[], int size)
{
    if (size <= 1)
        return;
    auto q_sort = [&](auto &self, int left, int right) mutable -> void
    {
        if (left >= right)
            return;

        int pivot = arr[left];
        int i = left - 1;
        int j = right + 1;

        while (true)
        {
            do
            {
                ++i;
            } while (arr[i] < pivot);
            do
            {
                --j;
            } while (arr[j] > pivot);

            if (i >= j)
                break;

            swap(arr[i], arr[j]);
        }
        self(self, left, j);
        self(self, j + 1, right);
    };
    q_sort(q_sort, 0, size - 1);
}