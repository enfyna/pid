<p align="center">
    <img width=20% src=https://github.com/user-attachments/assets/c740045c-2967-49b0-8f2a-c95ca1f320d7>
</p>
<h1 align="center">
  PID
</h1>
<p align="center">
    <img src=https://img.shields.io/badge/raylib-ffffff.svg?style=for-the-badge&logo=c&logoColor=black&labelColor=white>
    <img src=https://img.shields.io/badge/c-%2300599C.svg?style=for-the-badge&logo=c&logoColor=white>
</p>
<br><br>
<p align="center">
    Proportional
    Integral
    Derivative
</p>
<p align="center">
    <img width=70% src=https://github.com/user-attachments/assets/6089a471-094c-4e4d-b39a-41132ba6a336>
</p>

> [!NOTE]
> PID formulas are taken from here: [Pid Controller - How Machines Steer](https://www.feval.ca/posts/PID/)

> [!NOTE]
> Car physics formulas are taken from here: [Car Physics For Games](https://asawicki.info/Mirror/Car%20Physics%20for%20Games/Car%20Physics%20for%20Games.html)
> (Weight transfer and slip ratio isnt implemented.) 


# Installation

1. Clone the repo.

        git clone https://github.com/enfyna/pid
        cd pid

> [!NOTE]
> If you have missing dependencies try to install them.
2. Build the project.

            make

5. Run

       ./build/main 


# Parameters

When you run the program from the command line you can pass these parameters: 

Car:

    --car <"toyota_trueno" | "corvette_c5">

Defaults to Corvette C5.

Graph Margin:

    --graph-margin <int>


# Keyboard Bindings

| Key | Action | Key | Action |
| :---: | :---: | :---: | :---: |
| W | Increase P | R | Increase D |
| S | Decrease P | F | Decrease D |
| X | P = 0 | V | D = 0 |
| E | Increase I | J | Increase Target Speed |
| D | Decrease I | K | Decrease Target Speed |
| C | I = 0 | 1 | Round Target Speed |

# Develop

### Clangd

generate compile_commands.json:

    bear -- make
