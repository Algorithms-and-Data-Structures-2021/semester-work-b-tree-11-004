from random import randint


def values_generator(set_quantity: int, min_value: int = 0, max_value: int = 100000000,
                     min_elements_amount: int = 1000000, max_elements_amount: int = 10):
    for _ in range(set_quantity):
        elements_quantity = min_elements_amount
        with open("1000000.csv", "a") as inp:
            for i in range(elements_quantity):
                value = randint(min_value, max_value)
                inp.write(str(value) + " ")
            inp.write("\n")

if __name__ == '__main__':
    values_generator(1)