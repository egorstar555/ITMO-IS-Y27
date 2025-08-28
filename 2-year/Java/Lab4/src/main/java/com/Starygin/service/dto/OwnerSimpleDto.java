package com.Starygin.service.dto;

import java.time.LocalDate;

public record OwnerSimpleDto(Long id, String name, LocalDate birthday) {}