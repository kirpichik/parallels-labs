    }
    end  = __rdtsc();

    fprintf(stderr, "k = %d\n", k);
    printf("%.3lf\n", (double) (end - start) / iter_num);
    return (EXIT_SUCCESS);
}
