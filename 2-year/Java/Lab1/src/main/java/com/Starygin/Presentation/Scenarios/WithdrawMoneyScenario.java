package com.Starygin.Presentation.Scenarios;

import com.Starygin.Application.Contracts.UserService;
import com.Starygin.Presentation.Scenario;

import java.io.IOException;
import java.util.Scanner;

public class WithdrawMoneyScenario implements Scenario {
    private final UserService userService;

    public WithdrawMoneyScenario(UserService userService) {
        this.userService = userService;
    }

    @Override
    public String getName() {
        return "Withdraw Money";
    }

    @Override
    public void run() {
        System.out.println("Enter account id: ");
        Scanner scanner = new Scanner(System.in);

        try {
            long accountId = scanner.nextLong();
            System.out.println("Enter money amount: ");
            double amount = scanner.nextDouble();

            userService.withdrawMoney(accountId, amount);

            System.out.println("Successfully withdrawn money");

        } catch (IOException e) {
            System.out.println(e.getMessage());
        }
    }
}
