import cs50


def main():
    inpuT = input_fun()
    money = round(inpuT * 100)
    number = 0

    while money >= 25:
        money -= 25
        number = number + 1

    while money >= 10:
        money -= 10
        number = number + 1

    while money >= 5:
        money -= 5
        number = number + 1

    while money >= 1:
        money -= 1
        number = number + 1

    print(number)


def input_fun():
    while True:
        x = cs50.get_float("Change owed: ")
        if x > 0:
            break
    return x


main()
