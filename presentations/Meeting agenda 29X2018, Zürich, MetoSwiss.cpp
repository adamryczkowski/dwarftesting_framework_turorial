/*
1. When reduction be supported for GPU?
2. Can reduction have side-effects (modify the fields)?


Problems with the gridtools:

1. Very complicated syntax, compared to plain Fortran
   - It is not DSL. One must know quite advanced C++ to feel confident in the use of the Library. (IMO porting the library to C++14 or even C++17 probably still would not fix all the problems.)
     - API uses both type arguments and value arguments that puzzles people from Fortran
     - templated classes
     - template type arguments
     - redundancy repetitions
2. Long compilation time
3. Unacceptable memory consumption during compilation
4. Feature incomplete
   - Field size is runtime, so no opportunity to optimize fields with degenerated dimensions, and no compile time checks for problems with field dimensionality.
   - No checks for most of the compile time errors (list is long). And if they are, most of the time there are no usefull compiler messages.
   - No automatic computation of the halo margin in the stencil (only manual declaration).
   - No way to iterate in 2D, 1D or 4D.
   - No way to addresses field absolutely in the stencil (useful for vector of consts, or manual modification of the halo)
5. Even more feature incomplete:
   - No way to automatically create sibling stencils by permuatating indices of a given stencil.
6. API is still not stable. When can we expect GridTools 1.0?

*/


//Preferred API for gridtools 2.0, ported to C++ syntax: (although not possible to implement in the current C++ due to lack of any form of compile time code introspection)

constexpr auto dimX = auto_dim() + ascending();
//
//
//dimension - defines span of the axis. Two modes: relative to the field, and fixed (unrelated to the fields' axes)
//
//relative mode nouns:
//auto_dim - Axis along the fields, the same dimension that the field has. It means that if it is definition of the second axis,
//           it will take size of the second dimension of the fields, and in this case it is equivalent to `dim(1_c)`.
//dim(0_c) - first (0th) dimension of the input fields. Throws compilation error, if stencil used with a 
//           field that is bigger or smaller in this dimesion.
//relative mode adjectives:
//upper_margin(1_c) - adds 1 to the upper margin of the field, effectively allowing iterating over HALO. Negative values will reduce the iteration span.
//lower_margin(2_c) - adds 2 to the lower margin of the field.
//
//fixed mode nouns:
//fixed_dimension(0_c, 10_c) - fixed dimension that spans 11 pixels.
//
//
//direction - defines direction of iteration. Four constants:
//
//ascending - iterates from low values toward high values
//descending - iterates from high values toward low values
//parallel - iterates in parallel. Order is not defined
//no_iteration - declares that there will be no loop along this dimension (that is the default state). All relative accessors will throw compilation errors.




//This is the code for the stencil. It takes two arguments: in fields and inout fields
auto mystencil1 []() {
	constexpr auto dimX = auto_dim() + ascending();
	constexpr auto dimY = auto_dim() + parallel();
	constexpr auto dimZ = auto_dim() + upper_margin(1) + lower_margin(1)) + parallel(); //iterates in parallel along the third axis, but not only the inner range, but also 
	//one pixel on each HALO direction. Required if we want to perform something on the HALO lines directly.
	constexpr auto dimF = fixed_dimension(0_c, 4_c); //fixed-size dimension. Implicitely 
	constexpr auto loop = declare_loop(dimX, dimY, dimZ, dimF);

	constexpr auto i = dimX.accessor(0_c);
	constexpr auto j = dimY.accessor(1_c);
	constexpr auto k = dimZ.accessor(2_c);
	constexpr auto c = dimF.accessor(3_c); //4th fixed dimension

	in_field x_in = "x_in"_s; //lack of compile-time introspection in C++ forces us to repeat the name of the variable as the compile-time string, at least as long as we want to match the field by name.
	inout_field x_out = "x_out"_s;
	in_field v1 = "v1"_s;
	in_field v2 = "v2"_s;
	in_field v3 = "v3"_s;
	in_field const = "consts"_s;
	
	code [=]() {
		x_out() = x_in(i=i.value()) + v1(x+1) + v2(y+1) + v3(z+1) + const(c=0); //the actual stencil code
};
	//x_in() is equivalent to x_in(i) and to x_in(i=i.value()). i.value() returns runtime integer with the index of the iteration.
	return make_stencil(//in case of make_reduction, it needs an extra argument for the reducing function, just like in the GridTools.
	             "stencil_name"_s, //needed for meaningfull reports only. Perhaps should be optional.
	             code, //code of the stencil
	             loop, //definition how to iterate
	             make_tuple(x_in, x_out, v1, v2, v3, const) //list of the fields. Compile-time checks, if the keys are unique.
	             );
};

my_multistage = make_multistage(//Function inspects all the loop definitions inside the stencils and decides if it can merge them into a one loop. 
                                //If not - throws a meaningfull error. (I know this part is hard, but is doable in compile time, just maybe not in the current C++)
                                //Function also calculates the halo margin, by examining how the relative accessors are used. If the margin exceeds the 
                                //halo of the given fields, it throws compile time error.
                                make_tuple(mystencil1, mystencil2, mystencil3, myreduction),//Just stencils. Order matters. Last one can be a reduction.
                                make_field_dictionary(myfields, make_pair("consts"_s, field_of_consts)) //composes a compile-time dictionary of fields. 
                                //each argument can be either a dictionary or a pair of (key, field).
                                //Keys must be unique, and each name declared in the stencil must be matched with the 
                                );
auto reduction_ans = my_multistage.run(); //optionally I might manually upload the fields to the GPU using `field_collection.make_sure_uploaded()`
//reduction_ans - is either void, of the result of the reduction in the last stage. In future it might be a tuple of results comming from 

