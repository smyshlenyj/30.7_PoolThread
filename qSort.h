#pragma once
#include <future>
#include <vector>

RequestHandler rh;
std::future<void> fut;

void quickSort(std::shared_ptr<int[]> array, long left, long right)
{
  if (left >= right)
    return;
  long leftBound = left;
  long rightBound = right;
  long middle = array[(static_cast<ptrdiff_t>(leftBound) + rightBound) / 2];
  do
  {
    while (array[leftBound] < middle)
      ++leftBound;
    while (array[rightBound] > middle)
      --rightBound;
    if (leftBound <= rightBound)
    {
      std::swap(array[leftBound], array[rightBound]);
      ++leftBound;
      --rightBound;
    }
  } while (leftBound <= rightBound);
  quickSort(array, left, rightBound);
  quickSort(array, leftBound, right);
}

long partition(std::shared_ptr<int[]> array, long start, long end)
{
  long pivot = array[start];

  long count = 0;
  for (long i = start + 1; i <= end; i++)
  {
    if (array[i] <= pivot)
      count++;
  }

  long pivotIndex = start + count;
  std::swap(array[pivotIndex], array[start]);

  long i = start, j = end;

  while (i < pivotIndex && j > pivotIndex)
  {

    while (array[i] <= pivot)
    {
      i++;
    }

    while (array[j] > pivot)
    {
      j--;
    }

    if (i < pivotIndex && j > pivotIndex)
    {
      std::swap(array[i++], array[j--]);
    }
  }

  return pivotIndex;
}

void quickSortMulti(std::shared_ptr<int[]> array, long left, long right)
{
  std::thread::id this_id = std::this_thread::get_id ();
  //std::cout << this_id << std::endl;
  if (left < right)
  {
    long p = partition(array, left, right);

    if (right - left > 10'000 )
    {
      std::packaged_task<void()> task([=]() { rh.pushRequest(quickSortMulti, array, left, p - 1); });
fut = task.get_future();
        task();

      //rh.pushRequest(quickSortMulti, array, left, p - 1);
      quickSortMulti(array, p + 1, right);
    }

    else
    {
      quickSortMulti(array, left, p - 1);
      quickSortMulti(array, p + 1, right);
    }
  }
}