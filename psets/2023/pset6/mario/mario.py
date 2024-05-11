import cs50


def main():
    h = get_int()

    for i in range(h):
        for jok in range(h - i - 1):
            print(" ", end="")
    for jeb in range(i + 1):
            print("#", end="")
        print("")


def get_int():
    while True:
        x = cs50.get_int("Height: ")
        if x > 0 and x < 9:
            break
    return x


main()
