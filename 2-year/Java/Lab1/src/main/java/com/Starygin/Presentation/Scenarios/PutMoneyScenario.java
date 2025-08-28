package com.Starygin.Presentation.Scenarios;

import com.Starygin.Application.Contracts.UserService;
import com.Starygin.Presentation.Scenario;

import java.io.IOException;
import java.util.Scanner;

public class PutMoneyScenario implements Scenario {
    private final UserService userService;

    public PutMoneyScenario(UserService userService) {
        this.userService = userService;
    }

    @Override
    public String getName() {
        return "Put Money";
    }

    @Override
    public void run() {
        System.out.println("Enter account id: ");
        Scanner scanner = new Scanner(System.in);

        try {
            long accountId = scanner.nextLong();
            System.out.println("Enter money amount: ");
            double amount = scanner.nextDouble();

            userService.putMoney(accountId, amount);

            System.out.println("Successfully put money");

        } catch (IOException e) {
            System.out.println(e.getMessage());
        }
    }
}
