package com.Starygin.service.exceptions;

public class OwnerIdMismatchException extends RuntimeException {
    public OwnerIdMismatchException() {
        super("Owner id mismatch");
    }
}
