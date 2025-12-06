The program is a calculator designed to help SAR Analog-to-digital converter designers to design and to validate their work.

There are 5 functions available, each carries out a different task.

Function 1: Calculate for single MOS parameters. It will ask you for values of a single MOS and give you calculated parameters. Note that it will accept very unreasonable values, because real commercial EDAs also allow you to do that. Therefore it is the user's responsibility to ensure you are entering reasonable values! However, basic input validations are in place so you can't enter strings for numbers.

Function 2: Similar to function 1, but this time it uses some of the values from function 1 and computes parameters for peripheral components such as capacitor values. Note that you must fill in certain numbers from function 1 in order for function 2 to work, failure to do so will trigger a warning.

Function 3: Perform ADC metrics calculation. The calculation will depend on values in function 1 and 2, so it follows a similar requirement for function 2.

Function 4: Read / write CSV file, either stores the values in the program or writes them out. Notice that if you input a file to the system, it must match the given format or else the program won't allow the input action.

Function 5: If you know what you are doing, you can modify single device parameter to get to where you want quickly without going into detail of each function. However if you are not careful with what you are entering, this may result in weird numerical results, which you might not want to do unless you are very familiar with the topic!