#include <iostream>
#include "RequestHandler.h"
#include "qSort.h"
#include <thread>

int main()
{
   // std::srand(0);
   long arrSize = 9'000'000;
   std::shared_ptr<int[]> array(new int[arrSize]);
   std::shared_ptr<int[]> arrayCopy(new int[arrSize]);
   for (long i = 0; i < arrSize; ++i)
   {
      int random = array[i] = std::rand() % 10;
      array[i] = arrayCopy[i] = random;
   }
   /*
     std::cout << "Original array: " << std::endl;

      for (int i = 0; i < arrSize; ++i)
         std::cout << array[i] << " ";
      std::cout << std::endl;
   */

   quickSort(array, 0, arrSize);
   quickSortMulti(arrayCopy, 0, arrSize);
   std::this_thread::sleep_for(std::chrono::seconds(5)); // Workaround to make threads finish their work

   bool singleError = false;
   bool multiError = false;

   for (long i = 0; i < arrSize - 1; ++i)
   {
      if (array[i] > array[i + 1])
      {
         singleError = true;
         std::cout << "Single thread unsorted" << std::endl;
         break;
      }

      if (arrayCopy[i] > arrayCopy[i + 1])
      {
         multiError = true;
         std::cout << "Multi thread unsorted" << std::endl;
         break;
      }
   }
   /*
  std::cout << "\nSorted array: " << std::endl;
   for (int i = 0; i < arrSize; ++i)
      std::cout << array[i] << "";

  std::cout << "\nSorted arrayCopy: " << std::endl;
   for (int i = 0; i < arrSize; ++i)
      std::cout << arrayCopy[i] << "";
*/

   std::cout << "\n Single thread error status: " << singleError << std::endl;
   std::cout << "\n Multi thread error status: " << multiError << std::endl;

   return 0;
}