package com.Starygin.Application.Models;

public record UserOperation(long accountId, double moneyAmount, String operationName) { }