# PineappleExpress

## Description
System that will (hopefully) accelerate SHA-256 on an FPGA.

Amazon F1 instances will be used to test the system on real hardware. CPU and FPGA will communicate through DRAM connected to PCIe with Amazon's [EDMA driver](https://github.com/aws/aws-fpga/tree/master/sdk/linux_kernel_drivers/edma).

A double buffer will be used to allow the CPU and FPGA to write and read buffers in parallel.

## Documentation
Code is mostly self-documenting.

Notes from daily and weekly scrum together with other various information can be found in our [blog](https://pineappleblogg.wordpress.com/).


## Code standard

### Names

Snake case for everything except class names. Class names use camel case

### Spacing

Indentation level is two spaces.

### Brackets

```

void foo(){
  if(!coffee){
    make_coffee();
  }
}

```


### Pointers

```

int *bar

```

NOT

```

int* bar

```

### Pull requests

Merge is used for pull requests.


### Other
Follow the Google c++ style guide
