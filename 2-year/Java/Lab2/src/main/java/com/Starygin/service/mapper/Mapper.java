package com.Starygin.service.mapper;

public interface Mapper<E, D> {
    public E toEntity(D dto);

    public D toDto(E entity);
}