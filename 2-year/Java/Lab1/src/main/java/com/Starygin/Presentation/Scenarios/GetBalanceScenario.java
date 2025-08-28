package com.Starygin.Presentation.Scenarios;

import com.Starygin.Application.Contracts.UserService;
import com.Starygin.Presentation.Scenario;

import java.io.IOException;
import java.util.Scanner;

public class GetBalanceScenario implements Scenario {
    private final UserService userService;

    public GetBalanceScenario(UserService userService) {
        this.userService = userService;
    }

    @Override
    public String getName() {
        return "Get Balance";
    }

    @Override
    public void run() {
        System.out.println("Enter account id: ");
        Scanner scanner = new Scanner(System.in);

        try {
            long accountId = scanner.nextLong();

            double balance = userService.getBalance(accountId);

            System.out.println("Your balance is: " + balance);

        } catch (IOException e) {
            System.out.println(e.getMessage());
        }
    }
}
