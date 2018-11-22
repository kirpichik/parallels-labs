
    }
    end  = __rdtsc();
    if (min == 0 || min > end - start)
      min = end - start;
    }
    printf("%.3lf\n", (double) (end- start) / iter_num);
    //aver += ((end + 0.) - start);
    if (arr2[ARRAY_SIZE / 2] == 12314)
      printf("\n");
    //printf("%f\n", aver / iter_num);
    return (EXIT_SUCCESS);
}

