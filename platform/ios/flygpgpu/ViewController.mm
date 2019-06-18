//
//  ViewController.m
//  flygpgpu
//
//  Created by qiruisun on 2019/6/17.
//  Copyright © 2019 detu. All rights reserved.
//

#import "ViewController.h"
#include <string>
#include "fly_demo/demo1/demo1.hpp"
@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    NSString* url = [[NSBundle mainBundle] pathForResource:@"vulkan_kernel" ofType:@"glsl"];;


    std::string kernel_file = [url UTF8String];
    fly::ComputeAPI kernel_type =  fly::ComputeAPI::Vulkan;
    std::string kernel_function = "main";
    
    demo1_running_kernel(0,0,kernel_file, kernel_function, kernel_type);
    
    // Do any additional setup after loading the view, typically from a nib.
}


@end
