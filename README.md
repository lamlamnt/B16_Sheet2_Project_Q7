# B16_Sheet2_Project_Q7
Automated robot delivery system
<br /> In src, to compile:
<br /> g++ -I ../include main.cpp system.cpp -o ../build/main
<br /> In build, to run: ./main
<br /> Haven't implemented the function to generate edges randomly and a function to optimally (or almost optimally) compute the delivery path. 
<br /> Right now, the implemented delivery plan is just to take a full separate trip (using the shortest path) for each customer.
<br /> A slightly better approach is to deliver to customers with 3 baskets first. And then pair up customers with 1 basket and 2 baskets. Deliver to customers that are close together on the same trip. 
