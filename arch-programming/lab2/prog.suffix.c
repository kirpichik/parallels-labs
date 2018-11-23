    }
    end  = __rdtsc();

    fprintf(stderr, "pos = %d\n", pos);
    printf("%.3lf\n", (double) (end - start) / iter_num);
    return (EXIT_SUCCESS);
}
