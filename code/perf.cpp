#include <windows.h>
#include <iostream>

int main()
{
    LARGE_INTEGER Freq;

    if (QueryPerformanceFrequency(&Freq))
    {
        LARGE_INTEGER Ctr1, Ctr2;
        QueryPerformanceCounter(&Ctr1);
        QueryPerformanceCounter(&Ctr2);
        std::cout << "QueryPerformanceCounter minimum resolution: 1/" << Freq.QuadPart << " sec" << std::endl;
        std::cout << "API Overhead: " << double(Ctr2.QuadPart - Ctr1.QuadPart)*1000 / Freq.QuadPart << "milliseconds" << std::endl;
    }
    else
    {
        std::cerr << "High-resolution counter not supported." << std::endl;
        return 1;
    }

  const int N = 10;
  LARGE_INTEGER li[N];

  for (int i = 0; i < N; ++i)
  {
      Sleep(1000);
    ::QueryPerformanceCounter(&li[i]);
  }

  for (int i = 1; i < N; ++i)
  {
      std::cout << li[i].QuadPart << " diff is " << (li[i].QuadPart-li[i-1].QuadPart)*1000/Freq.QuadPart << "ms" << std::endl;
  }
}
